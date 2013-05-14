/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDijkstraGraphTraits.h,v $
  Language:  C++
  Date:
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDijkstraGraphTraits_h
#define __itkDijkstraGraphTraits_h

#include "itkDefaultGraphTraits.h"

namespace itk
{

/**
 */

template < typename TWeight = short, unsigned int VImageDimension = 3 >
class DijkstraGraphTraits : public DefaultGraphTraits< TWeight, TWeight >
{
public:
  typedef DijkstraGraphTraits                     Self;
  typedef DefaultGraphTraits<TWeight, TWeight>    Superclass;

  typedef Index< VImageDimension >                           IndexType;
  typedef TWeight                                            NodeWeightType;
  typedef TWeight                                            EdgeWeightType;
  typedef typename Superclass::NodeIdentifierType            NodeIdentifierType;
  typedef typename Superclass::EdgeIdentifierType            EdgeIdentifierType;
  typedef typename Superclass::EdgeIdentifierContainerType   EdgeIdentifierContainerType;

  struct  NodeType;
  typedef NodeType* NodePointerType;

  struct NodeType
    {
      NodeIdentifierType          Identifier;
      EdgeIdentifierContainerType IncomingEdges;
      EdgeIdentifierContainerType OutgoingEdges;
      NodeWeightType              Weight;
      IndexType                   ImageIndex;
      bool                        Visited;
      bool                        Added;
      NodeWeightType              AccumulatedWeight;
    };

  struct  EdgeType;
  typedef EdgeType* EdgePointerType;

  struct EdgeType
  {
    EdgeIdentifierType Identifier;
    NodeIdentifierType SourceIdentifier;
    NodeIdentifierType TargetIdentifier;
    EdgeIdentifierType ReverseEdgeIdentifier;
    EdgeWeightType     Weight;
    bool               OptimalEdge;
  };

};

} // end namespace itk

#endif
