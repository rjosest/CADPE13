/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkExtractLungLabelMapImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2010/12/20 21:25:08 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkExtractLungLabelMapImageFilter_txx
#define _itkExtractLungLabelMapImageFilter_txx

#include "itkExtractLungLabelMapImageFilter.h"


namespace itk
{


ExtractLungLabelMapImageFilter
::ExtractLungLabelMapImageFilter()
{
  this->m_RegionVec.push_back( UNDEFINEDREGION );

  REGIONANDTYPE regionTypeTemp;
    regionTypeTemp.lungRegionValue = UNDEFINEDREGION;
    regionTypeTemp.lungTypeValue   = UNDEFINEDTYPE;
 
  this->m_RegionAndTypeVec.push_back( regionTypeTemp );
}


void
ExtractLungLabelMapImageFilter
::GenerateData()
{
  this->InitializeMaps();

  //
  // Allocate the output buffer
  //
  this->GetOutput()->SetBufferedRegion( this->GetOutput()->GetRequestedRegion() );
  this->GetOutput()->Allocate();
  this->GetOutput()->FillBuffer( 0 );

  //
  // Now assign the regions and types in the output image based on the
  // mapping we determined in 'InitializeMaps'
  //
  OutputIteratorType oIt( this->GetOutput(), this->GetOutput()->GetBufferedRegion() );
  InputIteratorType iIt( this->GetInput(), this->GetInput()->GetBufferedRegion() );

  oIt.GoToBegin();
  iIt.GoToBegin();
  while ( !iIt.IsAtEnd() )
    {
    if ( iIt.Get() != 0 )
      {
      unsigned short outputValue = this->m_ValueToValueMap[ iIt.Get() ];

      oIt.Set( outputValue );
      }
    
    ++oIt;
    ++iIt;
    }
}


void
ExtractLungLabelMapImageFilter
::InitializeMaps()
{
  typedef std::pair< unsigned char, unsigned char > UCHAR_PAIR;

  //
  // Create the mappings for each region to one of the regions that
  // the user has specified. Note that m_RegionVec will at least
  // contain UNDEFINEDREGION
  //
  for ( int i=0; i<this->m_LungConventions.LungRegionVector.size(); i++ )
    {
    unsigned char tempRegion = this->m_LungConventions.LungRegionVector[i];

    bool regionMapped = false;

    while ( !regionMapped )
      {
      for ( int j=0; j<this->m_RegionVec.size(); j++ )
        {
        if ( tempRegion == this->m_RegionVec[j] )
          {
          this->m_RegionMap.insert( UCHAR_PAIR( this->m_LungConventions.LungRegionVector[i], this->m_RegionVec[j] ) );

          regionMapped = true;
          }        
        }
      tempRegion = this->m_LungConventions.LungRegionHierarchyMap[ tempRegion ];
      }
    }

  //
  // Create the mappings for each region to one of the regions that
  // the user has specified as a region-type pair. Note that
  // m_RegionAndTypeVec will at least contain the UNDEFINEDREGION,
  // UNDEFINEDTYPE pair
  //
  for ( int i=0; i<this->m_LungConventions.LungRegionVector.size(); i++ )
    {
    unsigned char tempRegion = this->m_LungConventions.LungRegionVector[i];

    bool regionMapped = false;
    while ( !regionMapped )
      {
      for ( int j=0; j<this->m_RegionAndTypeVec.size(); j++ )
        {
        if ( tempRegion == this->m_RegionAndTypeVec[j].lungRegionValue )
          {
          this->m_RegionMapForRegionTypePairs.insert( UCHAR_PAIR( this->m_LungConventions.LungRegionVector[i], this->m_RegionAndTypeVec[j].lungRegionValue ) );

          regionMapped = true;
          }        
        }
      tempRegion = this->m_LungConventions.LungRegionHierarchyMap[ tempRegion ];
      }
    }

  //
  // Iterate through the input image and create a list of all values.
  // Sort and unique this list and then compute a mapping of the
  // values to the appropriate region/type pairs.  Using this map will
  // greatly speed computation later.
  //
  std::list< unsigned short > valueList;
  valueList.push_back( 0 );

  InputIteratorType iIt( this->GetInput(), this->GetInput()->GetBufferedRegion() );

  iIt.GoToBegin();
  while ( !iIt.IsAtEnd() )
    {
    if ( iIt.Get() != 0 )
      {
      valueList.push_back( iIt.Get() );
      }

    ++iIt;
    }

  valueList.unique();
  valueList.sort();
  valueList.unique();

  std::list< unsigned short >::iterator listIt;
  listIt = valueList.begin();

  for ( int i=0; i<valueList.size(); i++, listIt++ )
    {
    unsigned char inputType   = this->m_LungConventions.GetLungTypeFromValue( *listIt );
    unsigned char inputRegion = this->m_LungConventions.GetLungRegionFromValue( *listIt );

    REGIONANDTYPE mappedRegionAndType;
      mappedRegionAndType.lungRegionValue = 0;
      mappedRegionAndType.lungTypeValue   = 0;

    //
    // Set the mapped region
    //
    mappedRegionAndType.lungRegionValue = this->m_RegionMap[ inputRegion ];

    //
    // Set the mapped type 
    //
    for ( int j=0; j<this->m_TypeVec.size(); j++ )
      {
      if ( static_cast< int >( inputType ) == static_cast< int >( this->m_TypeVec[j] ) )
        {      
        mappedRegionAndType.lungTypeValue = this->m_TypeVec[j];
        }
      }

    //
    // If there is a type/region pair, it will take precedence for the
    // mapping
    //
    unsigned char mappedRegionForRegionTypePairs = this->m_RegionMapForRegionTypePairs[ inputRegion ];

    if ( inputType != UNDEFINEDTYPE && inputRegion != UNDEFINEDREGION )
      {
      for ( int i=0; i<this->m_RegionAndTypeVec.size(); i++ )
        {
        if ( inputType == this->m_RegionAndTypeVec[i].lungTypeValue && mappedRegionForRegionTypePairs == this->m_RegionAndTypeVec[i].lungRegionValue )
          {
          mappedRegionAndType.lungTypeValue   = inputType;
          mappedRegionAndType.lungRegionValue = mappedRegionForRegionTypePairs;
          }
        }
      }

    unsigned short mappedValue = this->m_LungConventions.GetValueFromLungRegionAndType( mappedRegionAndType.lungRegionValue, 
                                                                                        mappedRegionAndType.lungTypeValue );

    this->m_ValueToValueMap.insert( std::pair< unsigned short, unsigned short >( *listIt, mappedValue ) );
    }
}


/**
 * Standard "PrintSelf" method
 */
void
ExtractLungLabelMapImageFilter
::PrintSelf(
  std::ostream& os, 
  Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}

} // end namespace itk

#endif
