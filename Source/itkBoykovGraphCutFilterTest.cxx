/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: BoykovGraphCutFilter.cxx,v $
  Language:  C++
  Date:      $Date: $
  Version:   $Revision: $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkBoykovGraphTraits.h"
#include "itkGraph.h"
#include "itkBoykovImageToGraphFunctor.h"
#include "itkBoykovAlphaExpansionMRFImageFilter.h"

#include "itkImage.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"
#include "vnl/vnl_math.h"
#include <string>
#include <vector>

#define pi 3.1415926535897932384626433832795

template<unsigned int ImageDimension>
int itkBoykovGraphCutFilterTest( int argc, char * argv[])
{

  /** specify the average and standard deviation of each class  */
  double mu1, mu2, mu3, mu4;
  double s1, s2, s3, s4;
  bool exclude = false;
  unsigned int Nlabels;

  std::string filename = std::string( argv[2] );

  unsigned int whichCase = 0;
  if( filename.find( "r85slice.png" ) != std::string::npos )
    {
    whichCase = 1;
    }
  else if( filename.find( "tools.jpg" ) != std::string::npos )
    {
    whichCase = 2;
    }
  else if( filename.find( "peppers.jpg" ) != std::string::npos )
    {
    whichCase = 3;
    }
  else if( filename.find( "ct_scan.jpg" ) != std::string::npos )
    {
    whichCase = 4;
    }
  else if( filename.find( "lungs.nii.gz" ) != std::string::npos )
    {
    whichCase = 5;
    }

  switch ( whichCase )
    {
    case 1:
      Nlabels = 3;
      exclude = true;
      mu1 =  50.0;   s1 = 20.0;
      mu2 = 150.0;   s2 = 20.0;
      mu3 = 240.0;   s3 = 20.0;
      break;
    case 2:
      Nlabels = 3;
      mu1 = 15.0;   s1 = 20.0;
      mu2 = 65.0;   s2 = 20.0;
      mu3 = 100.0;  s3 = 20.0;
      break;
    case 3:
      Nlabels = 4;
      mu1 =   0.0;   s1 = 20.0;
      mu2 =  54.0;   s2 = 20.0;
      mu3 = 170.0;   s3 = 20.0;
      mu4 = 190.0;   s4 = 20.0;
      break;
    case 4:
      Nlabels = 4;
      mu1 =   0.0;   s1 = 20.0;
      mu2 = 163.0;   s2 = 20.0;
      mu3 = 218.0;   s3 = 20.0;
      mu4 = 250.0;   s4 = 20.0;
      break;
    case 5:
      Nlabels = 2;
      mu1 = 200.0;   s1 = 100.0;
      mu2 = 800.0;   s2 = 100.0;
      break;
    default:
      std::cerr << "Error: incorrect option selected." << std::endl;
      return EXIT_FAILURE;
      break;
    }

  typedef itk::Image<int,  ImageDimension> ImageType;
  typedef typename ImageType::IndexType    IndexType;
  typedef itk::ImageFileReader<ImageType>  ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[2] );
  reader->Update();

  typedef itk::Image<double, ImageType::ImageDimension> LikelihoodImageType;

  typedef itk::ImageRegionIteratorWithIndex<LikelihoodImageType> IteratorType;

  typedef itk::CastImageFilter<ImageType, LikelihoodImageType> CasterType;
  typename CasterType::Pointer caster = CasterType::New();
  caster->SetInput( reader->GetOutput() );
  caster->Update();

  typename LikelihoodImageType::Pointer input = caster->GetOutput();
  typename LikelihoodImageType::Pointer label1_image
    = LikelihoodImageType::New();
  typename LikelihoodImageType::Pointer label2_image
    = LikelihoodImageType::New();
  typename LikelihoodImageType::Pointer label3_image
    = LikelihoodImageType::New();
  typename LikelihoodImageType::Pointer label4_image
    = LikelihoodImageType::New();

  label1_image->SetRegions( reader->GetOutput()->GetLargestPossibleRegion() );
  label1_image->SetOrigin( reader->GetOutput()->GetOrigin() );
  label1_image->SetSpacing( reader->GetOutput()->GetSpacing() );
  label1_image->SetDirection( reader->GetOutput()->GetDirection() );
  label1_image->Allocate();
  label2_image->SetRegions( reader->GetOutput()->GetLargestPossibleRegion() );
  label2_image->SetOrigin( reader->GetOutput()->GetOrigin() );
  label2_image->SetSpacing( reader->GetOutput()->GetSpacing() );
  label2_image->SetDirection( reader->GetOutput()->GetDirection() );
  label2_image->Allocate();
  label3_image->SetRegions( reader->GetOutput()->GetLargestPossibleRegion() );
  label3_image->SetOrigin( reader->GetOutput()->GetOrigin() );
  label3_image->SetSpacing( reader->GetOutput()->GetSpacing() );
  label3_image->SetDirection( reader->GetOutput()->GetDirection() );
  label3_image->Allocate();
  label4_image->SetRegions( reader->GetOutput()->GetLargestPossibleRegion() );
  label4_image->SetOrigin( reader->GetOutput()->GetOrigin() );
  label4_image->SetSpacing( reader->GetOutput()->GetSpacing() );
  label4_image->SetDirection( reader->GetOutput()->GetDirection() );
  label4_image->Allocate();

  IteratorType label1_it( label1_image,
    label1_image->GetLargestPossibleRegion() );
  IteratorType label2_it( label2_image,
    label2_image->GetLargestPossibleRegion() );
  IteratorType label3_it( label3_image,
    label3_image->GetLargestPossibleRegion() );
  IteratorType label4_it( label4_image,
    label4_image->GetLargestPossibleRegion() );

  for( label1_it.GoToBegin(); !label1_it.IsAtEnd(); ++label1_it )
    {
    label1_it.Set( vcl_exp( -vcl_pow( ( mu1 -
      input->GetPixel( label1_it.GetIndex() ) ), 2 )/( 2 * vcl_pow( s1, 2 ) ) )
      /( vcl_sqrt( 2 * pi ) * s1 ) );
    }
  for( label2_it.GoToBegin(); !label2_it.IsAtEnd(); ++label2_it )
    {
    label2_it.Set( vcl_exp( -vcl_pow( ( mu2 -
      input->GetPixel( label2_it.GetIndex() ) ), 2 )/( 2 * vcl_pow( s2, 2 ) ) )
      /( vcl_sqrt( 2 * pi ) * s2 ) );
    }
  if( Nlabels > 2 )
    {
    for( label3_it.GoToBegin(); !label3_it.IsAtEnd(); ++label3_it )
      {
      label3_it.Set( vcl_exp( -vcl_pow( ( mu3 -
        input->GetPixel( label3_it.GetIndex() ) ), 2 )
        /( 2 * vcl_pow( s3, 2 ) ) )/( vcl_sqrt( 2 * pi ) * s3 ) );
      }
    if( Nlabels > 3 )
      {
      for( label4_it.GoToBegin(); !label4_it.IsAtEnd(); ++label4_it )
        {
        label4_it.Set( vcl_exp( -vcl_pow( ( mu4 -
          input->GetPixel( label4_it.GetIndex() ) ), 2 )
          /( 2 * vcl_pow( s4, 2 ) ) ) /( vcl_sqrt( 2 * pi ) * s4 ) );
        }
      }
    }

  typedef itk::BoykovGraphTraits<short, ImageDimension> GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>                   GraphType;

  typedef itk::BoykovImageToGraphFunctor
    <LikelihoodImageType, GraphType> FunctorType;
  typename FunctorType::Pointer BoykovFunctor = FunctorType::New();
  BoykovFunctor->SetLambda( 10.0 );
  BoykovFunctor->SetSigma( 10.0 );
  BoykovFunctor->SetExcludeBackground( exclude );
  BoykovFunctor->SetBackgroundValue( 0 );
  BoykovFunctor->SetRadius( 1 );
  BoykovFunctor->ActivateAllNeighbors();

//  BoykovFunctor->ActivateIndex( 1 );
//  BoykovFunctor->ActivateIndex( 3 );
//  BoykovFunctor->ActivateIndex( 4 );
//  BoykovFunctor->ActivateIndex( 5 );
//  BoykovFunctor->ActivateIndex( 7 );

  typedef itk::BoykovAlphaExpansionMRFImageFilter
     <LikelihoodImageType, GraphTraitsType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetRandomizeInitialLabeling( false );
  filter->SetNumberOfClasses( Nlabels );
  filter->SetInput( input );
  filter->SetImageToGraphFunctor( BoykovFunctor );

  /**
   * The zeroth label is reserved for the background.
   */
  filter->SetLikelihoodImage( 1, label1_image );
  filter->SetLikelihoodImage( 2, label2_image );
  if( Nlabels > 2 )
    {
    filter->SetLikelihoodImage( 3, label3_image );
    if( Nlabels > 3 )
      {
      filter->SetLikelihoodImage( 4, label4_image );
      }
    }

  // This section is used to illustrate the mechanism where we force
  // certain pixel values to have a specific labeling.
  if( whichCase == 3 )
    {
    IndexType index;
    typename FilterType::IndexContainerType label1_indices;
    typename FilterType::IndexContainerType label2_indices;

    for( unsigned int i = 32; i <= 72; i++ )
      {
      for( unsigned int j = 86; j <= 124; j++ )
        {
        index[0] = i;
        index[1] = j;
        label2_indices.push_back( index );
        }
      }

    for( unsigned int i = 5; i <= 30; i++ )
      {
      for( unsigned int j = 5; j <= 30; j++ )
        {
        index[0] = i;
        index[1] = j;
        label1_indices.push_back( index );
        }
      }

    filter->SetIndexContainer( 1, label1_indices );
    filter->SetIndexContainer( 2, label2_indices );
    }
    
  try
    {
    filter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "BoykovAlphaExpansionMRFImageFilter exception thrown." 
      << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    exit( EXIT_FAILURE );
    }

  typedef itk::Image<int,  ImageDimension>     LabelImageType;
  typedef itk::ImageFileWriter<LabelImageType> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[3] );
  writer->SetInput( filter->GetOutput() );
  writer->Write();

  /*
  for( unsigned int i = 1; i <= NumberOfLabels; i++ )
    {
    itk::ImageFileWriter<LikelihoodImageType>::Pointer
    writer = itk::ImageFileWriter<LikelihoodImageType>::New();
    writer->SetFileName( "" );
    writer->SetInput( filter->GetPosteriorProbabilityImage( i ) );
    writer->Write();
    }
  */

  return EXIT_SUCCESS;
}

int main( int argc, char *argv[] )
{
  if( argc < 4 )
    {
    std::cout << argv[0] << " ImageDimension inputImage outputImage"
      << std::endl;
    exit( 0 );
    }

  switch( atoi( argv[1] ) )
    {
    case 2:
      itkBoykovGraphCutFilterTest<2>( argc, argv );
      break;
    case 3:
      itkBoykovGraphCutFilterTest<3>( argc, argv );
      break;
    default:
      std::cerr << "Unsupported dimension" << std::endl;
      exit( EXIT_FAILURE );
    }
}
