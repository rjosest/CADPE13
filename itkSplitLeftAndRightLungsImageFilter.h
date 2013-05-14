/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSplitLeftAndRightLungsImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2011/07/18 21:14:35 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSplitLeftAndRightLungsImageFilter_h
#define __itkSplitLeftAndRightLungsImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkExtractImageFilter.h"
#include "itkLungConventions.h"
#include "itkDijkstraGraphTraits.h"
#include "itkGraph.h" 
#include "itkImageToGraphFilter.h"
#include "itkDijkstraImageToGraphFunctor.h"
#include "itkDijkstraMinCostPathGraphToGraphFilter.h"


namespace itk
{
/** \class SplitLeftAndRightLungsImageFilter
 * \brief This filter takes as input a 3D lung label map image (with
 * labelings assumed to adhere to the conventions described in
 * itkLungConventions.h. The output of this filter is a lung label map
 * image with the left and right lungs split. No relabeling is
 * performed. 
 */
template <class TInputImage>
class ITK_EXPORT SplitLeftAndRightLungsImageFilter :
    public ImageToImageFilter< TInputImage, itk::Image< unsigned short, 3 > >
{
public:
  /** Extract dimension from input and output image. */
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension );
  itkStaticConstMacro( OutputImageDimension, unsigned int, 3 );

  /** Convenient typedefs for simplifying declarations. */
  typedef TInputImage                       InputImageType;
  typedef itk::Image< unsigned short, 3 >   OutputImageType;

  /** Standard class typedefs. */
  typedef SplitLeftAndRightLungsImageFilter                      Self;
  typedef ImageToImageFilter< InputImageType, OutputImageType >  Superclass;
  typedef SmartPointer< Self >                                   Pointer;
  typedef SmartPointer< const Self >                             ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SplitLeftAndRightLungsImageFilter, ImageToImageFilter);
  
  /** Image typedef support. */
  typedef unsigned short                        LabelMapPixelType;
  typedef itk::Image< LabelMapPixelType, 3 >    LabelMapType;
  typedef typename InputImageType::PixelType    InputPixelType;
  typedef OutputImageType::PixelType            OutputPixelType;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef OutputImageType::RegionType           OutputImageRegionType;
  typedef typename InputImageType::SizeType     InputSizeType;

  /** By default, the left-right lung splitting routine makes
   *  assumptions to make the process as fast as possible.  In some
   *  cases, however, this can result in left and right lungs that are
   *  still merged.  By setting 'AggressiveLeftRightSplitter' to true,
   *  the splitting routing will take longer, but will be more robust. */
  itkSetMacro( AggressiveLeftRightSplitter, bool ); 
  itkGetMacro( AggressiveLeftRightSplitter, bool );

  /** In order to split the left and right lungs, a min cost path
   *  algorithm is used.  To do this, a section of the image is
   *  converted to a graph and weights are assigned to the indices
   *  based on an exponential function ( f=A*exp{t/tau) ).  For the
   *  task of splitting the lungs, it's assumed that dark voxels are
   *  to be penalized much more than bright voxels.  For images
   *  ranging from -1024 to 1024, the default
   *  'ExponentialTimeConstant' and 'ExponentialCoefficient' values of
   *  -700 and 200 have been found to work well. If you find that the
   *  lungs are merged after running this filter, it migh help to
   *  double check the values you're setting for these parameters */
  itkSetMacro( ExponentialTimeConstant, double );
  itkGetMacro( ExponentialTimeConstant, double );

  /** See not for 'ExponentialTimeConstant' above */
  itkSetMacro( ExponentialCoefficient, double );
  itkGetMacro( ExponentialCoefficient, double );

  /** If the left and right lungs are merged in a certain section, 
   * graph methods are used to find a min cost path (i.e. the brightest
   * path) that passes through the merge region. This operation returns
   * a set of indices corresponding to points along the path. When the
   * lungs are actually split in two, a radius (essentially an erosion
   * radius) is used to separate the lungs.  The larger the radius, the
   * more aggressive the splitting. The default value is 3. */
  itkSetMacro( LeftRightLungSplitRadius, int );
  itkGetMacro( LeftRightLungSplitRadius, int );

  /** Use this method to get the vector of indices that were removed
   *  during the splitting process. Pass a pointer to an empty
   *  vector. This function will fill the vector with the erased
   *  indices */
  void GetRemovedIndices( std::vector< LabelMapType::IndexType >* );

  void SetLungLabelMap( LabelMapType::Pointer );

  void PrintSelf( std::ostream& os, Indent indent ) const;

protected:
  typedef itk::Image< LabelMapPixelType, 2 >         LabelMapSliceType;
  typedef typename itk::Image< InputPixelType, 2 >   InputSliceType;
  typedef typename InputSliceType::Pointer           InputSlicePointerType;
  typedef LabelMapSliceType::IndexType               LabelMapSliceIndexType;

  typedef itk::Image< InputPixelType, 2 >                                                        InputImageSliceType;
  typedef itk::ConnectedComponentImageFilter< LabelMapSliceType, LabelMapSliceType >             ConnectedComponent2DType;
  typedef itk::ImageRegionIteratorWithIndex< LabelMapType >                                      LabelMapIteratorType;
  typedef itk::ImageRegionConstIterator< InputImageType >                                        InputIteratorType;
  typedef itk::ImageRegionIteratorWithIndex< LabelMapSliceType >                                 LabelMapSliceIteratorType;
  typedef itk::ExtractImageFilter< InputImageType, InputImageSliceType >                         InputExtractorType;
  typedef itk::ExtractImageFilter< LabelMapType, LabelMapSliceType >                             LabelMapExtractorType;
  typedef unsigned long                                                                          GraphTraitsScalarType;
  typedef itk::DijkstraGraphTraits< GraphTraitsScalarType, 2 >                                   GraphTraitsType;
  typedef itk::Graph< GraphTraitsType >                                                          GraphType;
  typedef itk::ImageToGraphFilter< InputSliceType, GraphType >                                   GraphFilterType;
  typedef itk::DijkstraImageToGraphFunctor< InputSliceType, GraphType >                          FunctorType;
  typedef itk::DijkstraMinCostPathGraphToGraphFilter< GraphType, GraphType >                     MinPathType;

  SplitLeftAndRightLungsImageFilter();
  virtual ~SplitLeftAndRightLungsImageFilter() {}

  void SetRegion( OutputImageType::IndexType, int );
  unsigned char GetType( OutputImageType::IndexType );

  void ExtractLabelMapSlice( LabelMapType::Pointer, LabelMapSliceType::Pointer, int );

  std::vector< LabelMapSliceIndexType > GetMinCostPath( InputSlicePointerType, LabelMapSliceIndexType, LabelMapSliceIndexType );

  bool GetLungsMergedInSliceRegion( int, int, int, int, int );

  void GenerateData();

private:
  SplitLeftAndRightLungsImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  LabelMapType::Pointer m_LungLabelMap;

  std::vector< LabelMapType::IndexType >  m_RemovedIndices;
  LungConventions                         m_LungConventions;
  double                                  m_ExponentialCoefficient;
  double                                  m_ExponentialTimeConstant;
  bool                                    m_AggressiveLeftRightSplitter;
  int                                     m_LeftRightLungSplitRadius;
};
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSplitLeftAndRightLungsImageFilter.txx"
#endif

#endif
