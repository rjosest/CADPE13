/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDijkstraMinCostPathGraphToGraphFilter.h,v $
  Language:  C++
  Date:      $$
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDijkstraMinCostPathGraphToGraphFilter_h
#define __itkDijkstraMinCostPathGraphToGraphFilter_h

#include "itkGraphToGraphFilter.h"

namespace itk
{

/** \class DijkstraMinCostPathGraphToGraphFilter
 * \brief Computes the minimum cost path between two specified points
 * using Dijkstra's algorithm.
 *
 * \ingroup GraphFilters
 *
 */
template <class TInputGraph, class TOutputGraph>
class ITK_EXPORT DijkstraMinCostPathGraphToGraphFilter : public GraphToGraphFilter< TInputGraph, TOutputGraph >
{
public:
  /** Standard class typedefs. */
  typedef DijkstraMinCostPathGraphToGraphFilter            Self;
  typedef GraphToGraphFilter< TInputGraph, TOutputGraph >  Superclass;
  typedef SmartPointer< Self >                             Pointer;
  typedef SmartPointer< const Self >                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( DijkstraMinCostPathGraphToGraphFilter, GraphToGraphFilter );

  /** Some convenient typedefs. */
  typedef TInputGraph                                   InputGraphType;
  typedef TOutputGraph                                  OutputGraphType;
  typedef typename InputGraphType::Pointer              InputGraphPointer;
  typedef typename InputGraphType::NodeIdentifierType   InputNodeIdentifierType;
  typedef typename OutputGraphType::NodeIdentifierType  OutputNodeIdentifierType;
  typedef typename InputGraphType::NodeIterator         InputNodeIteratorType;
  typedef typename InputGraphType::EdgeIterator         InputEdgeIteratorType;
  typedef typename InputGraphType::NodeWeightType       InputNodeWeightType;
  typedef typename InputGraphType::NodeType             InputNodeType;
  typedef typename InputGraphType::NodePointerType      InputNodePointerType;
  typedef typename OutputGraphType::NodePointerType     OutputNodePointerType;

  itkSetMacro( StartNode, InputNodeIdentifierType );
  itkGetMacro( StartNode, InputNodeIdentifierType );

  itkSetMacro( EndNode, InputNodeIdentifierType );
  itkGetMacro( EndNode, InputNodeIdentifierType );
  

protected:
  DijkstraMinCostPathGraphToGraphFilter();
  ~DijkstraMinCostPathGraphToGraphFilter() {};

  void GenerateData();

private:
  DijkstraMinCostPathGraphToGraphFilter( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  bool GetIDOfLowestCostUnvisited( InputNodeIdentifierType&, std::vector< InputNodeIdentifierType >& );

  InputNodeIdentifierType m_StartNode;
  InputNodeIdentifierType m_EndNode;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkDijkstraMinCostPathGraphToGraphFilter.txx"
#endif

#endif
