/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGraphTest.cxx,v $
  Language:  C++
  Date:      $Date: $
  Version:   $Revision: $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "itkGraph.h"
#include "itkDefaultGraphTraits.h"

int main( int argc, char * argv[] )
{
  typedef itk::DefaultGraphTraits<short, short> GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>           GraphType;
  GraphType::Pointer graph = GraphType::New();
  graph->DebugOn();

  typedef GraphType::NodeType             NodeType;
  typedef GraphType::EdgeType             EdgeType;
  typedef GraphType::NodePointerType      NodePointerType;
  typedef GraphType::EdgePointerType      EdgePointerType;
  typedef GraphType::NodeIterator         NodeIteratorType;
  typedef GraphType::EdgeIterator         EdgeIteratorType;
  typedef GraphType::NodeWeightType       NodeWeightType;
  typedef GraphType::EdgeWeightType       EdgeWeightType;
  typedef GraphType::NodeIdentifierType   NodeIdentifierType;
  typedef GraphType::EdgeIdentifierType   EdgeIdentifierType;

  // Create graph: use Pointers
  NodePointerType Nodes[4];
  EdgePointerType Edges[10];

  for( unsigned int i = 0; i < 4; i++ )
    {
    graph->CreateNewNode( 2 );
    }
  for( unsigned int i = 0; i < 4; i++ )
    {
    Nodes[i] = graph->GetNodePointer( i );
    }

  graph->CreateNewEdge( Nodes[0], Nodes[1], 2 );
  graph->CreateNewEdge( Nodes[1], Nodes[0], 2 );
  graph->CreateNewEdge( Nodes[1], Nodes[2], 2 );
  graph->CreateNewEdge( Nodes[2], Nodes[1], 2 );
  graph->CreateNewEdge( Nodes[0], Nodes[3], 2 );
  graph->CreateNewEdge( Nodes[3], Nodes[0], 2 );
  graph->CreateNewEdge( Nodes[1], Nodes[3], 2 );
  graph->CreateNewEdge( Nodes[3], Nodes[1], 2 );
  graph->CreateNewEdge( Nodes[2], Nodes[3], 2 );
  graph->CreateNewEdge( Nodes[3], Nodes[2] );

  for( unsigned int i = 0; i < 10; i++ )
    {
    Edges[i] = graph->GetEdgePointer( i );
    }

  graph->SetNodeWeight( Nodes[3], 2 );
  graph->AddNodeWeight( Nodes[3], 0 );
  graph->SetEdgeWeight( Edges[9], 2 );
  graph->AddEdgeWeight( Edges[9], 0 );

  /** Set the reverse edges */
  graph->SetAllReverseEdges();

  std::cout << "Total number of nodes: "
    << graph->GetTotalNumberOfNodes() << std::endl;
  std::cout << "Total number of edges: "
    << graph->GetTotalNumberOfEdges() << std::endl;

  if( graph->GetTotalNumberOfEdges() != 10
    || graph->GetTotalNumberOfNodes() != 4 )
    {
    return EXIT_FAILURE;
    }

  NodeIteratorType nit( graph );
  for( nit.GoToBegin(); !nit.IsAtEnd(); ++nit )
    {
    NodePointerType node = nit.GetPointer();
    NodeIdentifierType Id = graph->GetNodeIdentifier( node );
    node = graph->GetNodePointer( Id );
    NodeWeightType w = graph->GetNodeWeight( node );
    //
    //std::cout << "Node Id: " << Id << std::endl 
    //  << "   Weight = " << w << std::endl;
    if ( w != 2 )
      {
      return EXIT_FAILURE;
      }
    }


  EdgeIteratorType eit( graph );
  for ( eit.GoToBegin(); !eit.IsAtEnd(); ++eit )
    {
    EdgePointerType edge = eit.GetPointer();
    EdgeIdentifierType Id = graph->GetEdgeIdentifier( edge );
    edge = graph->GetEdgePointer( Id );
    EdgeWeightType w = graph->GetEdgeWeight( edge );

    if ( w != 2 )
      {
      return EXIT_FAILURE;
      }

    NodeIdentifierType EdgeSourceId
      = graph->GetNodeIdentifier( graph->GetSourceNode( edge ) );
    NodeIdentifierType EdgeTargetId
      = graph->GetNodeIdentifier( graph->GetTargetNode( edge ) );

    EdgePointerType reverse
      = graph->GetReverseEdgePointer( graph->GetEdgeIdentifier( edge ) );
    EdgeIdentifierType ReverseId = graph->GetEdgeIdentifier( reverse );
    reverse = graph->GetReverseEdgePointer( edge );
    NodeIdentifierType ReverseSourceId
      = graph->GetNodeIdentifier( graph->GetSourceNodePointer( reverse ) );
    NodeIdentifierType ReverseTargetId
      = graph->GetNodeIdentifier( graph->GetTargetNodePointer( reverse ) );

    if( EdgeSourceId != ReverseTargetId ||
        EdgeTargetId != ReverseSourceId)
      {
      return EXIT_FAILURE;
      }
    std::cout << "Edge Id: " << Id << std::endl
              << "   Weight     = " << w << std::endl
              << "   Source Id: = " << EdgeSourceId << std::endl
              << "   Target Id: = " << EdgeTargetId << std::endl;
    std::cout << "   Reverse Edge Id: " << ReverseId << std::endl
              << "      Weight      = " 
              << graph->GetEdgeWeight(reverse) << std::endl
              << "      Source Id:  = " << ReverseSourceId << std::endl
              << "      Target Id:  = " << ReverseTargetId << std::endl;
    }

  graph->Print( std::cout );
  std::cout << "Clearing graph." << std::endl;
  graph->Clear();

  // Create graph: use references

  NodeType nodes[4];
  EdgeType edges[10];

  for( unsigned int i = 0; i < 4; i++ )
    {
    nodes[i] = *graph->CreateNewNode( 2 );
    }

  edges[0] = *graph->CreateNewEdge( nodes[0], nodes[1], 2 );
  edges[1] = *graph->CreateNewEdge( nodes[1], nodes[0], 2 );
  edges[2] = *graph->CreateNewEdge( nodes[1], nodes[2], 2 );
  edges[3] = *graph->CreateNewEdge( nodes[2], nodes[1], 2 );
  edges[4] = *graph->CreateNewEdge( nodes[0], nodes[3], 2 );
  edges[5] = *graph->CreateNewEdge( nodes[3], nodes[0], 2 );
  edges[6] = *graph->CreateNewEdge( nodes[1], nodes[3], 2 );
  edges[7] = *graph->CreateNewEdge( nodes[3], nodes[1], 2 );
  edges[8] = *graph->CreateNewEdge( nodes[2], nodes[3], 2 );
  edges[9] = *graph->CreateNewEdge( nodes[3], nodes[2] );

  graph->SetNodeWeight( nodes[3], 2 );
  graph->AddNodeWeight( nodes[3], 0 );
  graph->SetEdgeWeight( edges[9], 2 );
  graph->AddEdgeWeight( edges[9], 0 );
  graph->SetNodeWeight( graph->GetNodeIdentifier( nodes[3] ), 2 );
  graph->AddNodeWeight( graph->GetNodeIdentifier( nodes[3] ), 0 );
  graph->SetEdgeWeight( graph->GetEdgeIdentifier( edges[9] ), 2 );
  graph->AddEdgeWeight( graph->GetEdgeIdentifier( edges[9] ), 0 );

  /** Set the reverse edges */
  graph->SetAllReverseEdges();

  std::cout << "Total number of nodes: "
    << graph->GetTotalNumberOfNodes() << std::endl;
  std::cout << "Total number of edges: "
    << graph->GetTotalNumberOfEdges() << std::endl;
  if( graph->GetTotalNumberOfEdges() != 10
    || graph->GetTotalNumberOfNodes() != 4 )
    {
    return EXIT_FAILURE;
    }

  for( nit.GoToBegin(); !nit.IsAtEnd(); ++nit )
    {
    NodeType node = nit.Get();
    NodeIdentifierType Id = graph->GetNodeIdentifier( node );
    if( nit.GetIdentifier() != Id )
      {
      return EXIT_FAILURE;
      }
    node = graph->GetNode( Id );
    NodeWeightType w = graph->GetNodeWeight( node );
    if(w != 2)
      {
      return EXIT_FAILURE;
      }
    std::cout << "Node Id: " << Id << std::endl
      << "   Weight = " << w << std::endl;
    }

  for( eit.GoToBegin(); !eit.IsAtEnd(); ++eit )
    {
    EdgeType edge = eit.Get();
    EdgeIdentifierType Id = graph->GetEdgeIdentifier( edge );
    if( eit.GetIdentifier() != Id )
      {
      return EXIT_FAILURE;
      }
    edge = graph->GetEdge( Id );
    EdgeWeightType w = graph->GetEdgeWeight( edge );
    NodeIdentifierType EdgeSourceId
      = graph->GetNodeIdentifier( graph->GetSourceNode( edge ) );
    NodeIdentifierType EdgeTargetId
      = graph->GetNodeIdentifier( graph->GetTargetNode( edge ) );

    if( w != 2 )
      {
      return EXIT_FAILURE;
      }

    EdgeType reverse = graph->GetReverseEdge(graph->GetEdgeIdentifier( edge ) );
    EdgeIdentifierType ReverseId = graph->GetEdgeIdentifier( reverse );
    NodeIdentifierType ReverseSourceId
      = graph->GetNodeIdentifier( graph->GetSourceNode( &reverse ) );
    NodeIdentifierType ReverseTargetId
      = graph->GetNodeIdentifier( graph->GetTargetNode( &reverse ) );
    reverse = graph->GetEdge( ReverseSourceId, ReverseTargetId );
    NodeType Source = graph->GetSourceNode( ReverseId );
    NodeType Target = graph->GetTargetNode( ReverseId );

    if( EdgeSourceId != ReverseTargetId ||
      EdgeTargetId != ReverseSourceId )
      {
      return EXIT_FAILURE;
      }
    std::cout << "Edge Id: " << Id << std::endl
              << "   Weight     = " << w << std::endl
              << "   Source Id: = " << EdgeSourceId << std::endl
              << "   Target Id: = " << EdgeTargetId << std::endl;
    std::cout << "   Reverse Edge Id: " << ReverseId << std::endl
              << "      Weight      = " << graph->GetEdgeWeight( reverse ) 
              << std::endl
              << "      Source Id:  = " << ReverseSourceId << std::endl
              << "      Target Id:  = " << ReverseTargetId << std::endl;
    }

  graph->Print( std::cout );
  graph->Clear();
  std::cout << "Clearing graph." << std::endl;
  if( graph->GetTotalNumberOfNodes() != 0
    || graph->GetTotalNumberOfEdges() != 0 )
    {
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
