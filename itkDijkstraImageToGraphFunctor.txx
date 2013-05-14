/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDijkstraImageToGraphFunctor.txx,v $
  Language:  C++
  Date:      $Date: 2009/08/20 15:50:22 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDijkstraImageToGraphFunctor_txx
#define __itkDijkstraImageToGraphFunctor_txx

#include "itkDijkstraImageToGraphFunctor.h"
#include "vnl/vnl_math.h"

namespace itk
{

template<typename TInputImage, typename TOutputGraph>
DijkstraImageToGraphFunctor<TInputImage, TOutputGraph>
::DijkstraImageToGraphFunctor()
{
  this->m_UpperThreshold = itk::NumericTraits< PixelType >::max();
  this->m_LowerThreshold = itk::NumericTraits< PixelType >::min();
  this->m_ExponentialCoefficient  = 50;
  this->m_ExponentialTimeConstant = 700;
  this->m_LinearBasedCostAssignment      = false;
  this->m_SigmoidBasedCostAssignment     = false;
  this->m_ExponentialBasedCostAssignment = true;
  this->m_SigmoidScale     = 5.0;
  this->m_SigmoidShift     = -800;
  this->m_SigmoidSteepness = 0.05;
}

template<typename TInputImage, typename TOutputGraph>
typename DijkstraImageToGraphFunctor<TInputImage, TOutputGraph>::EdgeWeightType
DijkstraImageToGraphFunctor<TInputImage, TOutputGraph>
::GetEdgeWeight( IndexType idx1, IndexType idx2 )
{
  return static_cast< EdgeWeightType >( 0 );
}


template<typename TInputImage, typename TOutputGraph>
typename DijkstraImageToGraphFunctor<TInputImage, TOutputGraph>::NodeWeightType
DijkstraImageToGraphFunctor<TInputImage, TOutputGraph>
::GetNodeWeight( IndexType idx1 )
{
  double pixelValue = static_cast< double >( this->GetInput()->GetPixel( idx1 ) );

  double nodeWeight;

  if ( this->m_LinearBasedCostAssignment )
    {
    nodeWeight = pixelValue;
    }
  else if ( this->m_ExponentialBasedCostAssignment )
    {
    nodeWeight = this->m_ExponentialCoefficient*vcl_exp( pixelValue/this->m_ExponentialTimeConstant );
    }
  else
    {
    nodeWeight = this->m_SigmoidScale/( 1.0 + vcl_exp( -this->m_SigmoidSteepness*( pixelValue - this->m_SigmoidShift ) ) );
    }

  return static_cast< NodeWeightType >( nodeWeight );
}


template<typename TInputImage, typename TOutputGraph>
bool
DijkstraImageToGraphFunctor<TInputImage, TOutputGraph>
::IsPixelANode( IndexType idx1 )
{
  if ( this->GetInput()->GetPixel( idx1 ) >= this->m_LowerThreshold &&
       this->GetInput()->GetPixel( idx1 ) <= this->m_UpperThreshold )
    {
    return true;
    }

  return false;
}


template<typename TInputImage, typename TOutputGraph>
bool 
DijkstraImageToGraphFunctor<TInputImage, TOutputGraph>
::IsAnEdge( IndexType idx1, IndexType idx2 )
{
  return true;
}


template<typename TInputImage, typename TOutputGraph>
void
DijkstraImageToGraphFunctor<TInputImage, TOutputGraph>
::NormalizeGraph( NodeImageType *image, OutputGraphType *graph )
{

}


template<typename TInputImage, typename TOutputGraph>
void
DijkstraImageToGraphFunctor<TInputImage, TOutputGraph>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << " " << std::endl;
}


} // end namespace itk

#endif
