/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelLungRegionsImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2011/12/12 23:25:07 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkLabelLungRegionsImageFilter_txx
#define _itkLabelLungRegionsImageFilter_txx

#include "itkLabelLungRegionsImageFilter.h"
#include "itkImageFileWriter.h"


namespace itk
{

LabelLungRegionsImageFilter
::LabelLungRegionsImageFilter()
{
  this->m_HeadFirst               =  true;
  this->m_Supine                  =  true;
  this->m_LabelLungThirds         =  false;
  this->m_LabelLeftAndRightLungs  =  false;
  this->m_LabelingSuccess         =  false;
  this->m_NumberLungVoxels        =  0;
}


void
LabelLungRegionsImageFilter
::GenerateData()
{
  //
  // Allocate space for the output image
  //
  Superclass::InputImageConstPointer inputPtr  = this->GetInput();
  Superclass::OutputImagePointer     outputPtr = this->GetOutput(0);
    outputPtr->SetRequestedRegion( inputPtr->GetRequestedRegion() );
    outputPtr->SetBufferedRegion( inputPtr->GetBufferedRegion() );
    outputPtr->SetLargestPossibleRegion( inputPtr->GetLargestPossibleRegion() );
    outputPtr->Allocate();
    outputPtr->FillBuffer( 0 );

  //
  // Start by filling the output image with the WHOLELUNG region at
  // every location where the input image has a lung region set. Begin
  // by determining which of the input values correspond to a defined
  // lung region.
  //
  std::list< unsigned short > inputValuesList;

  InputIteratorType iIt( this->GetInput(), this->GetInput()->GetBufferedRegion() );

  iIt.GoToBegin();
  while ( !iIt.IsAtEnd() )
    {
    if ( iIt.Get() != 0 )
      {
      inputValuesList.push_back( iIt.Get() );
      }

    ++iIt;
    }
  
  inputValuesList.unique();
  inputValuesList.sort();
  inputValuesList.unique();

  std::vector< unsigned short > definedLungRegionValuesVec;

  std::list< unsigned short >::iterator listIt;

  std::map< unsigned short, unsigned char > valueToTypeMap; 

  for ( listIt = inputValuesList.begin(); listIt != inputValuesList.end(); listIt++ )
    {
    unsigned char lungRegion = this->m_LungConventions.GetLungRegionFromValue( *listIt );
    unsigned char lungType   = this->m_LungConventions.GetLungTypeFromValue( *listIt );

    valueToTypeMap[ *listIt ] = lungType;

    if ( lungRegion != 0 )
      {
      definedLungRegionValuesVec.push_back( *listIt );
      }
    }

  //
  // Now that we've collected all the input values that correspond to
  // defined lung regions, we can initialize the output image with the
  // WHOLELUNG region
  //
  unsigned short wholeLungLabel = this->m_LungConventions.GetValueFromLungRegionAndType( WHOLELUNG, UNDEFINEDTYPE );

  LabelMapIteratorType oIt( this->GetOutput(), this->GetOutput()->GetBufferedRegion() );

  oIt.GoToBegin();
  iIt.GoToBegin();
  while ( !oIt.IsAtEnd() )
    {
    if ( iIt.Get() != 0 )
      {
      for ( unsigned int i=0; i<definedLungRegionValuesVec.size(); i++ )
        {
        if ( iIt.Get() == definedLungRegionValuesVec[i] )
          {
          oIt.Set( wholeLungLabel );

          this->m_NumberLungVoxels++;
          }
        }
      }

    ++oIt;
    ++iIt;
    }

  if ( this->m_LabelLungThirds || this->m_LabelLeftAndRightLungs )
    {
    this->m_LabelingSuccess = this->LabelLeftAndRightLungs();
    }
  if ( this->m_LabelLungThirds && this->m_LabelingSuccess )
    {
    this->SetLungThirds();
    }

  //
  // Now set the types from the input image 
  //
  iIt.GoToBegin();
  oIt.GoToBegin();
  while ( !oIt.IsAtEnd() )
    {
    if ( iIt.Get() != 0 )
      {
      unsigned short lungTypeValue = valueToTypeMap[iIt.Get()];

      if ( lungTypeValue != UNDEFINEDTYPE )
        {
        this->SetType( oIt.GetIndex(), lungTypeValue ); 
        }
      }

    ++iIt;
    ++oIt;
    }
}


void
LabelLungRegionsImageFilter
::SetType( OutputImageType::IndexType index, int lungTypeValue )
{
  unsigned short currentValue = this->GetOutput()->GetPixel( index );

  //
  // Get the binary representation of the current value
  //
  int currentPlaces[16];
  for ( int i=0; i<16; i++ )
    {
    currentPlaces[i] = 0;
    }

  for ( int i=15; i>=0; i-- )
    {
    int power = static_cast< int >( pow( 2, i ) );

    if ( power <= currentValue )
      {
      currentPlaces[i] = 1;
      
      currentValue = currentValue % power;
      }
    }

  //
  // Get the binary representation of the type to set
  //
  int typeValue = lungTypeValue;

  int typePlaces[8];
  for ( int i=0; i<8; i++ )
    {
    typePlaces[i] = 0;
    }

  for ( int i=7; i>=0; i-- )
    {
    int power = static_cast< int >( pow( 2, i ) );

    if ( power <= typeValue )
      {
      typePlaces[i] = 1;
      
      typeValue = typeValue % power;
      }
    }

  //
  // Compute the new value to assign to the label map voxel 
  //
  unsigned short newValue = 0;

  for ( int i=0; i<16; i++ )
    {
    if ( i < 8 )
      {
      newValue += static_cast< unsigned short >( currentPlaces[i] )*static_cast< unsigned short >( pow( 2, i ) );
      }
    else
      {
      newValue += static_cast< unsigned short >( typePlaces[i-8] )*static_cast< unsigned short >( pow( 2, i ) );
      }
    }

  this->GetOutput()->SetPixel( index, newValue );
}


/**
 * 
 */
bool
LabelLungRegionsImageFilter
::LabelLeftAndRightLungs()
{
  typedef itk::ImageFileWriter< LabelMapType > WriterType;

  //
  // First test if the input is already split into left and right. For
  // this condition to be true, all voxel regions must be labeled as
  // either left or right. If this is true, simply transfer the labels
  // to the output image.  
  //
  InputIteratorType iIt( this->GetInput(), this->GetInput()->GetBufferedRegion() );
  LabelMapIteratorType oIt( this->GetOutput(), this->GetOutput()->GetBufferedRegion() );

  bool leftLungFound     = false;
  bool rightLungFound    = false;
  bool nonLeftRightFound = false;

  iIt.GoToBegin();
  while ( !iIt.IsAtEnd() )
    {
    if ( iIt.Get() != 0 )
      {
      if ( iIt.Get() == static_cast< unsigned short >( LEFTLUNG ) )
        {
        leftLungFound = true;
        }
      else if ( iIt.Get() == static_cast< unsigned short >( RIGHTLUNG ) )
        {
        rightLungFound = true;
        }
      else
        {
        nonLeftRightFound = true;
        }
      }

    ++iIt;
    }

  if ( leftLungFound && rightLungFound && !nonLeftRightFound )
    {
    oIt.GoToBegin();
    iIt.GoToBegin();
    while ( !oIt.IsAtEnd() )
      {
      oIt.Set( iIt.Get() );

      ++oIt;
      ++iIt;
      }

//    std::cout << "---Appears to already be left lung / right lung..." << std::endl;

    return true;
    }

  LabelMapType::SizeType size = this->GetInput()->GetBufferedRegion().GetSize();

  //
  // Perform connected component analysis.
  //
  ConnectedComponentType::Pointer connectedComponent = ConnectedComponentType::New();
    connectedComponent->SetInput( this->GetOutput() );
    connectedComponent->FullyConnectedOn();
    connectedComponent->Update();

  RelabelComponentType::Pointer relabelComponent = RelabelComponentType::New();
    relabelComponent->SetInput( connectedComponent->GetOutput() );
    relabelComponent->Update();

  if ( relabelComponent->GetNumberOfObjects() <= 1 )
    {
//    std::cout << "---Only found one component..." << std::endl;
    return false;
    }

  //
  // The following containter will keep track of the x-coordinates for
  // all the components. Once tallied, we'll determine the center of
  // mass along the x-direction for each component. We will then
  // assign each component to left or right depending on whether it is
  // closer to the left or right edge of the bounding box.
  //
  std::vector< std::vector< unsigned short > > componentLocations;

  //
  // Begin by initializing the container. We want to have a vector for
  // each component in order to contain the x coordinates for each
  // component. 
  //
  for ( unsigned int i=0; i<relabelComponent->GetNumberOfObjects(); i++ )
    {
    std::vector< unsigned short > temp;
    componentLocations.push_back( temp );
    }

  //
  // Now loop over the relabled image and collect the x-coordinates for
  // each of the components. At the same time, determine the min and
  // max x coordinates.
  //
  unsigned short minX = size[0];
  unsigned short maxX = 0;

  LabelMapIteratorType rIt( relabelComponent->GetOutput(), relabelComponent->GetOutput()->GetBufferedRegion() );

//  std::cout << "---Writing relabeler..." << std::endl;
//   WriterType::Pointer writer1 = WriterType::New();
//   writer1->SetInput( relabelComponent->GetOutput() );
//   writer1->SetFileName( "/projects/lmi/people/jross/tmp/relabeler.nhdr" );
//   writer1->UseCompressionOn();
//   writer1->Update();

  rIt.GoToBegin();
  while ( !rIt.IsAtEnd() )
    {
    if ( rIt.Get() != 0 )
      {
      componentLocations[rIt.Get()-1].push_back( rIt.GetIndex()[0] );

      if ( rIt.GetIndex()[0] < minX )
        {
        minX = rIt.GetIndex()[0];
        }
      if ( rIt.GetIndex()[0] > maxX )
        {
        maxX = rIt.GetIndex()[0];
        }
      }

    ++rIt;
    }
  
//   std::cout << "---Min x:\t" << minX << std::endl;
//   std::cout << "---Max x:\t" << maxX << std::endl;

  //
  // Now compute the center of mass for each component and assign lung
  // region values for each component value
  //
  std::map<unsigned short, unsigned char> componentToLungRegionMap;

  for ( unsigned int i=0; i<relabelComponent->GetNumberOfObjects(); i++ )
    {
    double xInc = 0;

    for ( unsigned int x=0; x<componentLocations[i].size(); x++ )
      {
      xInc += static_cast< double >( componentLocations[i][x] );
      }

    double massCenter = xInc/static_cast<double>(componentLocations[i].size());

    //
    // We'll assign the component as left or right lung depending on
    // which bounding box edge it is closer to
    //
    if ( (this->m_HeadFirst && this->m_Supine) || (!this->m_HeadFirst && !this->m_Supine) )
      {
      if ( vcl_abs(massCenter-static_cast<double>(minX)) <= vcl_abs(massCenter-static_cast<double>(maxX)) )
        {
//        std::cout << "---Setting component " << i+1 << " to right lung (mass center is: " << massCenter << "..." << std::endl;
        componentToLungRegionMap[i+1] = static_cast<unsigned char>(RIGHTLUNG);
        }
      else
        {
//        std::cout << "---Setting component " << i+1 << " to left lung (mass center is: " << massCenter << "..." << std::endl;
        componentToLungRegionMap[i+1] = static_cast<unsigned char>(LEFTLUNG);
        }
      }
    else
      {
      if ( vcl_abs(massCenter-static_cast<double>(minX)) <= vcl_abs(massCenter-static_cast<double>(maxX)) )
        {
        componentToLungRegionMap[i+1] = static_cast<unsigned char>(LEFTLUNG);
        }
      else
        {
        componentToLungRegionMap[i+1] = static_cast<unsigned char>(RIGHTLUNG);
        }
      }
    }

  //
  // Now that we have the mapping from component value to lung region,
  // we can populate the output
  //
  unsigned short rightLungLabel = this->m_LungConventions.GetValueFromLungRegionAndType( RIGHTLUNG, UNDEFINEDTYPE );
  unsigned short leftLungLabel  = this->m_LungConventions.GetValueFromLungRegionAndType( LEFTLUNG, UNDEFINEDTYPE );

//   std::cout << "---Right lung label:\t" << rightLungLabel << std::endl;
//   std::cout << "---Left lung label:\t" << leftLungLabel << std::endl;

  rIt.GoToBegin();
  oIt.GoToBegin();
  while ( !oIt.IsAtEnd() )
    {
    if ( rIt.Get() != 0 )
      {
      if ( componentToLungRegionMap[rIt.Get()] == static_cast<unsigned char>(LEFTLUNG) )
        {
        oIt.Set( leftLungLabel );
        }
      else if ( componentToLungRegionMap[rIt.Get()] == static_cast<unsigned char>(RIGHTLUNG) )
        {
        oIt.Set( rightLungLabel );
        }
      }
    else
      {
      oIt.Set( 0 );
      }

    ++rIt;
    ++oIt;
    }

//   std::cout << "---Writing output before returning from itkLabelLungRegions..." << std::endl;
//   WriterType::Pointer writer2 = WriterType::New();
//   writer2->SetInput( this->GetOutput() );
//   writer2->UseCompressionOn();
//   writer2->SetFileName( "/projects/lmi/people/jross/tmp/atEndOfLabelingProcess.nhdr" );
//   writer2->Update();

  return true;
}


/**
 * Entering this method, the left and right lung designations have
 * been made.
 */
void
LabelLungRegionsImageFilter
::SetLungThirds() 
{
  unsigned short leftLowerThirdLabel   = this->m_LungConventions.GetValueFromLungRegionAndType( LEFTLOWERTHIRD, UNDEFINEDTYPE );
  unsigned short rightLowerThirdLabel  = this->m_LungConventions.GetValueFromLungRegionAndType( RIGHTLOWERTHIRD, UNDEFINEDTYPE );
  unsigned short leftMiddleThirdLabel  = this->m_LungConventions.GetValueFromLungRegionAndType( LEFTMIDDLETHIRD, UNDEFINEDTYPE );
  unsigned short rightMiddleThirdLabel = this->m_LungConventions.GetValueFromLungRegionAndType( RIGHTMIDDLETHIRD, UNDEFINEDTYPE );
  unsigned short leftUpperThirdLabel   = this->m_LungConventions.GetValueFromLungRegionAndType( LEFTUPPERTHIRD, UNDEFINEDTYPE );
  unsigned short rightUpperThirdLabel  = this->m_LungConventions.GetValueFromLungRegionAndType( RIGHTUPPERTHIRD, UNDEFINEDTYPE );

  int voxelTally = 0;

  LabelMapIteratorType oIt( this->GetOutput(), this->GetOutput()->GetBufferedRegion() );

  oIt.GoToBegin();
  while ( !oIt.IsAtEnd() )
    {
    if ( oIt.Get() != 0 )
      {
      voxelTally++;

      if ( voxelTally <= this->m_NumberLungVoxels/3 )
        {
        if ( this->m_HeadFirst )
          {
          if ( oIt.Get() == LEFTLUNG )
            {
            oIt.Set( leftLowerThirdLabel );
            }
          if ( oIt.Get() == RIGHTLUNG )
            {
            oIt.Set( rightLowerThirdLabel );
            }
          }
        else
          {
          if ( oIt.Get() == LEFTLUNG )
            {
            oIt.Set( leftUpperThirdLabel );
            }
          if ( oIt.Get() == RIGHTLUNG )
            {
            oIt.Set( rightUpperThirdLabel );
            }
          }
        }
      else if ( voxelTally <= 2*this->m_NumberLungVoxels/3 )
        {
        if ( oIt.Get() == LEFTLUNG )
          {
          oIt.Set( leftMiddleThirdLabel );
          }
        if ( oIt.Get() == RIGHTLUNG )
          {
          oIt.Set( rightMiddleThirdLabel );
          }
        }
      else
        {
        if ( this->m_HeadFirst )
          {
          if ( oIt.Get() == LEFTLUNG )
            {
            oIt.Set( leftUpperThirdLabel );
            }
          if ( oIt.Get() == RIGHTLUNG )
            {
            oIt.Set( rightUpperThirdLabel );
            }
          }
        else
          {
          if ( oIt.Get() == LEFTLUNG )
            {
            oIt.Set( leftLowerThirdLabel );
            }
          if ( oIt.Get() == RIGHTLUNG )
            {
            oIt.Set( rightLowerThirdLabel );
            }
          }
        }
      }

    ++oIt;
    }
}

  
/**
 * Standard "PrintSelf" method
 */
void
LabelLungRegionsImageFilter
::PrintSelf(
  std::ostream& os, 
  Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Printing itkLabelLungRegionsImageFilter: " << std::endl;
  os << indent << "HeadFirst: " << this->m_HeadFirst << std::endl;
  os << indent << "Supine: " << this->m_Supine << std::endl;
  os << indent << "LabelLungThirds: " << this->m_LabelLungThirds << std::endl;
  os << indent << "LabelLeftAndRightLungs: " << this->m_LabelLeftAndRightLungs << std::endl;
  os << indent << "NumberLungVoxels: " << this->m_NumberLungVoxels << std::endl;
}

} // end namespace itk

#endif
