#ifndef __itkLungConventions_h
#define __itkLungConventions_h

#include <string>
#include <map>
#include <vector>
#include <math.h>

/**
 *  Note that lung regions are inherently hierarchical.  If you add a
 *  region to the enumerated list below, you should also update the
 *  'LungRegionHierarchyMap' described below.  Additionally, the
 *  LungRegionVector should be updated in the constructor. Also need
 *  to update m_NumberOfEnumeratedLungTypes member variable .
 */
enum LungRegion { 
  UNDEFINEDREGION, 
  WHOLELUNG, 
  RIGHTLUNG, 
  LEFTLUNG, 
  RIGHTSUPERIORLOBE, 
  RIGHTMIDDLELOBE, 
  RIGHTINFERIORLOBE, 
  LEFTSUPERIORLOBE, 
  LEFTINFERIORLOBE, 
  LEFTUPPERTHIRD, 
  LEFTMIDDLETHIRD, 
  LEFTLOWERTHIRD,
  RIGHTUPPERTHIRD, 
  RIGHTMIDDLETHIRD, 
  RIGHTLOWERTHIRD 
};


/**
 *  If you add a type to the enumerated list here, you should also
 *  update the LungTypeVector below (in the class constructor). 
 *  Also need to update m_NumberOfEnumeratedLungTypes member variable
 *  and the 'LungTypeStringVector'.
 */
enum LungType { 
  UNDEFINEDTYPE, 
  NORMALPARENCHYMA, 
  AIRWAY, 
  VESSEL, 
  EMPHYSEMATOUS, 
  GROUNDGLASS, 
  RETICULAR, 
  NODULAR,
  OBLIQUEFISSURE,
  HORIZONTALFISSURE,
  MILDPARASEPTALEMPHYSEMA,
  MODERATEPARASEPTALEMPHYSEMA,
  SEVEREPARASEPTALEMPHYSEMA,
  MILDBULLA,
  MODERATEBULLA,
  SEVEREBULLA,
  MILDCENTRILOBULAREMPHYSEMA,
  MODERATECENTRILOBULAREMPHYSEMA,
  SEVERECENTRILOBULAREMPHYSEMA,
  MILDPANLOBULAREMPHYSEMA,
  MODERATEPANLOBULAREMPHYSEMA,
  SEVEREPANLOBULAREMPHYSEMA,
  AIRWAYWALLTHICKENING,
  AIRWAYCYLINDRICALDILATION,
  VARICOSEBRONCHIECTASIS,
  CYSTICBRONCHIECTASIS,
  CENTRILOBULARNODULE,
  MOSAICING,
  EXPIRATORYMALACIA,
  SABERSHEATH,
  OUTPOUCHING,
  MUCOIDMATERIAL,
  PATCHYGASTRAPPING,
  DIFFUSEGASTRAPPING,
  LINEARSCAR,
  CYST,
  ATELECTASIS,
  HONEYCOMBING,
  AIRWAYGENERATION0,
  AIRWAYGENERATION1,
  AIRWAYGENERATION2,
  AIRWAYGENERATION3,
  AIRWAYGENERATION4,
  AIRWAYGENERATION5,
  AIRWAYGENERATION6,
  AIRWAYGENERATION7,
  AIRWAYGENERATION8,
};


/**
 *  The following class will define the hierarchy among the various
 *  regions defined in 'LungRegion' above.  If a new region is added
 *  to the enumerated list above, the class below should be updated
 *  as well to reflect the update.  'LungRegionHierarchyMap' contains
 *  a mapping between all regions in the 'LungRegion' enumerated list
 *  and the region directly above it in the hierarchy.  This map is
 *  used by such classes as 'itkExtractLungLabelMapImageFilter'.
 */
class LungConventions
{
public:
  // TODO: Check proper destructor syntax
  ~LungConventions(){};
  LungConventions()
    {
      m_NumberOfEnumeratedLungRegions = 15;
      m_NumberOfEnumeratedLungTypes   = 47;

      typedef std::pair< unsigned char, unsigned char > Region_Pair;

      LungRegionHierarchyMap.insert( Region_Pair( static_cast< unsigned char >( LEFTSUPERIORLOBE ), 
                                                  static_cast< unsigned char >( LEFTLUNG ) ) );
      LungRegionHierarchyMap.insert( Region_Pair( static_cast< unsigned char >( LEFTINFERIORLOBE ), 
                                                  static_cast< unsigned char >( LEFTLUNG ) ) );
      LungRegionHierarchyMap.insert( Region_Pair( static_cast< unsigned char >( RIGHTSUPERIORLOBE ), 
                                                  static_cast< unsigned char >( RIGHTLUNG ) ) );
      LungRegionHierarchyMap.insert( Region_Pair( static_cast< unsigned char >( RIGHTMIDDLELOBE ), 
                                                  static_cast< unsigned char >( RIGHTLUNG ) ) );
      LungRegionHierarchyMap.insert( Region_Pair( static_cast< unsigned char >( RIGHTINFERIORLOBE ), 
                                                  static_cast< unsigned char >( RIGHTLUNG ) ) );
      LungRegionHierarchyMap.insert( Region_Pair( static_cast< unsigned char >( LEFTLUNG ), 
                                                  static_cast< unsigned char >( WHOLELUNG ) ) );
      LungRegionHierarchyMap.insert( Region_Pair( static_cast< unsigned char >( RIGHTLUNG ), 
                                                  static_cast< unsigned char >( WHOLELUNG ) ) );
      LungRegionHierarchyMap.insert( Region_Pair( static_cast< unsigned char >( LEFTUPPERTHIRD ), 
                                                  static_cast< unsigned char >( LEFTLUNG ) ) );
      LungRegionHierarchyMap.insert( Region_Pair( static_cast< unsigned char >( LEFTMIDDLETHIRD ), 
                                                  static_cast< unsigned char >( LEFTLUNG ) ) );
      LungRegionHierarchyMap.insert( Region_Pair( static_cast< unsigned char >( LEFTLOWERTHIRD ), 
                                                  static_cast< unsigned char >( LEFTLUNG ) ) );
      LungRegionHierarchyMap.insert( Region_Pair( static_cast< unsigned char >( RIGHTUPPERTHIRD ), 
                                                  static_cast< unsigned char >( RIGHTLUNG ) ) );
      LungRegionHierarchyMap.insert( Region_Pair( static_cast< unsigned char >( RIGHTMIDDLETHIRD ), 
                                                  static_cast< unsigned char >( RIGHTLUNG ) ) );
      LungRegionHierarchyMap.insert( Region_Pair( static_cast< unsigned char >( RIGHTLOWERTHIRD ), 
                                                  static_cast< unsigned char >( RIGHTLUNG ) ) );
      LungRegionHierarchyMap.insert( Region_Pair( static_cast< unsigned char >( WHOLELUNG ), 
                                                  static_cast< unsigned char >( UNDEFINEDREGION ) ) );

      LungRegionVector.push_back( static_cast< unsigned char >( UNDEFINEDREGION ) );
      LungRegionVector.push_back( static_cast< unsigned char >( WHOLELUNG ) );
      LungRegionVector.push_back( static_cast< unsigned char >( RIGHTLUNG ) );
      LungRegionVector.push_back( static_cast< unsigned char >( LEFTLUNG ) );
      LungRegionVector.push_back( static_cast< unsigned char >( RIGHTSUPERIORLOBE ) );
      LungRegionVector.push_back( static_cast< unsigned char >( RIGHTMIDDLELOBE ) );
      LungRegionVector.push_back( static_cast< unsigned char >( RIGHTINFERIORLOBE ) );
      LungRegionVector.push_back( static_cast< unsigned char >( LEFTSUPERIORLOBE ) );
      LungRegionVector.push_back( static_cast< unsigned char >( LEFTINFERIORLOBE ) );
      LungRegionVector.push_back( static_cast< unsigned char >( LEFTUPPERTHIRD ) );
      LungRegionVector.push_back( static_cast< unsigned char >( LEFTMIDDLETHIRD ) );
      LungRegionVector.push_back( static_cast< unsigned char >( LEFTLOWERTHIRD ) );
      LungRegionVector.push_back( static_cast< unsigned char >( RIGHTUPPERTHIRD ) );
      LungRegionVector.push_back( static_cast< unsigned char >( RIGHTMIDDLETHIRD ) );
      LungRegionVector.push_back( static_cast< unsigned char >( RIGHTLOWERTHIRD ) );

      LungTypeVector.push_back( static_cast< unsigned char >( UNDEFINEDTYPE ) );
      LungTypeVector.push_back( static_cast< unsigned char >( NORMALPARENCHYMA ) );
      LungTypeVector.push_back( static_cast< unsigned char >( AIRWAY ) );
      LungTypeVector.push_back( static_cast< unsigned char >( VESSEL ) );
      LungTypeVector.push_back( static_cast< unsigned char >( EMPHYSEMATOUS ) );
      LungTypeVector.push_back( static_cast< unsigned char >( GROUNDGLASS ) );
      LungTypeVector.push_back( static_cast< unsigned char >( RETICULAR ) );
      LungTypeVector.push_back( static_cast< unsigned char >( NODULAR ) ); 
      LungTypeVector.push_back( static_cast< unsigned char >( OBLIQUEFISSURE ) ); 
      LungTypeVector.push_back( static_cast< unsigned char >( HORIZONTALFISSURE ) ); 
      LungTypeVector.push_back( static_cast< unsigned char >( MILDPARASEPTALEMPHYSEMA ) ); 
      LungTypeVector.push_back( static_cast< unsigned char >( MODERATEPARASEPTALEMPHYSEMA ) );
      LungTypeVector.push_back( static_cast< unsigned char >( SEVEREPARASEPTALEMPHYSEMA ) );
      LungTypeVector.push_back( static_cast< unsigned char >( MILDBULLA ) );
      LungTypeVector.push_back( static_cast< unsigned char >( MODERATEBULLA ) );
      LungTypeVector.push_back( static_cast< unsigned char >( SEVEREBULLA ) );
      LungTypeVector.push_back( static_cast< unsigned char >( MILDCENTRILOBULAREMPHYSEMA ) );
      LungTypeVector.push_back( static_cast< unsigned char >( MODERATECENTRILOBULAREMPHYSEMA ) );
      LungTypeVector.push_back( static_cast< unsigned char >( SEVERECENTRILOBULAREMPHYSEMA ) );
      LungTypeVector.push_back( static_cast< unsigned char >( MILDPANLOBULAREMPHYSEMA ) );
      LungTypeVector.push_back( static_cast< unsigned char >( MODERATEPANLOBULAREMPHYSEMA ) );
      LungTypeVector.push_back( static_cast< unsigned char >( SEVEREPANLOBULAREMPHYSEMA ) );
      LungTypeVector.push_back( static_cast< unsigned char >( AIRWAYWALLTHICKENING ) );
      LungTypeVector.push_back( static_cast< unsigned char >( AIRWAYCYLINDRICALDILATION ) );
      LungTypeVector.push_back( static_cast< unsigned char >( VARICOSEBRONCHIECTASIS ) );
      LungTypeVector.push_back( static_cast< unsigned char >( CYSTICBRONCHIECTASIS ) );
      LungTypeVector.push_back( static_cast< unsigned char >( CENTRILOBULARNODULE ) );
      LungTypeVector.push_back( static_cast< unsigned char >( MOSAICING ) );
      LungTypeVector.push_back( static_cast< unsigned char >( EXPIRATORYMALACIA ) );
      LungTypeVector.push_back( static_cast< unsigned char >( SABERSHEATH ) );
      LungTypeVector.push_back( static_cast< unsigned char >( OUTPOUCHING ) );
      LungTypeVector.push_back( static_cast< unsigned char >( MUCOIDMATERIAL ) );
      LungTypeVector.push_back( static_cast< unsigned char >( PATCHYGASTRAPPING ) );
      LungTypeVector.push_back( static_cast< unsigned char >( DIFFUSEGASTRAPPING ) );
      LungTypeVector.push_back( static_cast< unsigned char >( LINEARSCAR ) );
      LungTypeVector.push_back( static_cast< unsigned char >( CYST ) );
      LungTypeVector.push_back( static_cast< unsigned char >( ATELECTASIS ) );
      LungTypeVector.push_back( static_cast< unsigned char >( HONEYCOMBING ) );
      LungTypeVector.push_back( static_cast< unsigned char >( AIRWAYGENERATION0 ) );
      LungTypeVector.push_back( static_cast< unsigned char >( AIRWAYGENERATION1 ) );
      LungTypeVector.push_back( static_cast< unsigned char >( AIRWAYGENERATION2 ) );
      LungTypeVector.push_back( static_cast< unsigned char >( AIRWAYGENERATION3 ) );
      LungTypeVector.push_back( static_cast< unsigned char >( AIRWAYGENERATION4 ) );
      LungTypeVector.push_back( static_cast< unsigned char >( AIRWAYGENERATION5 ) );
      LungTypeVector.push_back( static_cast< unsigned char >( AIRWAYGENERATION6 ) );
      LungTypeVector.push_back( static_cast< unsigned char >( AIRWAYGENERATION7 ) );
      LungTypeVector.push_back( static_cast< unsigned char >( AIRWAYGENERATION8 ) );

      LungRegionStringVector.push_back( "UNDEFINEDREGION" );
      LungRegionStringVector.push_back( "WHOLELUNG" ); 
      LungRegionStringVector.push_back( "RIGHTLUNG" ); 
      LungRegionStringVector.push_back( "LEFTLUNG" ); 
      LungRegionStringVector.push_back( "RIGHTSUPERIORLOBE" );
      LungRegionStringVector.push_back( "RIGHTMIDDLELOBE" ); 
      LungRegionStringVector.push_back( "RIGHTINFERIORLOBE" );
      LungRegionStringVector.push_back( "LEFTSUPERIORLOBE" ); 
      LungRegionStringVector.push_back( "LEFTINFERIORLOBE" );
      LungRegionStringVector.push_back( "LEFTUPPERTHIRD" ); 
      LungRegionStringVector.push_back( "LEFTMIDDLETHIRD" );
      LungRegionStringVector.push_back( "LEFTLOWERTHIRD" ); 
      LungRegionStringVector.push_back( "RIGHTUPPERTHIRD" );
      LungRegionStringVector.push_back( "RIGHTMIDDLETHIRD" ); 
      LungRegionStringVector.push_back( "RIGHTLOWERTHIRD" );

      LungTypeStringVector.push_back( "UNDEFINEDTYPE" );
      LungTypeStringVector.push_back( "NORMALPARENCHYMA" );
      LungTypeStringVector.push_back( "AIRWAY" );
      LungTypeStringVector.push_back( "VESSEL" );
      LungTypeStringVector.push_back( "EMPHYSEMATOUS" );
      LungTypeStringVector.push_back( "GROUNDGLASS" );
      LungTypeStringVector.push_back( "RETICULAR" );
      LungTypeStringVector.push_back( "NODULAR" ); 
      LungTypeStringVector.push_back( "OBLIQUEFISSURE" ); 
      LungTypeStringVector.push_back( "HORIZONTALFISSURE" ); 
      LungTypeStringVector.push_back( "MILDPARASEPTALEMPHYSEMA" ); 
      LungTypeStringVector.push_back( "MODERATEPARASEPTALEMPHYSEMA" );
      LungTypeStringVector.push_back( "SEVEREPARASEPTALEMPHYSEMA" );
      LungTypeStringVector.push_back( "MILDBULLA" );
      LungTypeStringVector.push_back( "MODERATEBULLA" );
      LungTypeStringVector.push_back( "SEVEREBULLA" );
      LungTypeStringVector.push_back( "MILDCENTRILOBULAREMPHYSEMA" );
      LungTypeStringVector.push_back( "MODERATECENTRILOBULAREMPHYSEMA" );
      LungTypeStringVector.push_back( "SEVERECENTRILOBULAREMPHYSEMA" );
      LungTypeStringVector.push_back( "MILDPANLOBULAREMPHYSEMA" );
      LungTypeStringVector.push_back( "MODERATEPANLOBULAREMPHYSEMA" );
      LungTypeStringVector.push_back( "SEVEREPANLOBULAREMPHYSEMA" );
      LungTypeStringVector.push_back( "AIRWAYWALLTHICKENING" );
      LungTypeStringVector.push_back( "AIRWAYCYLINDRICALDILATION" );
      LungTypeStringVector.push_back( "VARICOSEBRONCHIECTASIS" );
      LungTypeStringVector.push_back( "CYSTICBRONCHIECTASIS" );
      LungTypeStringVector.push_back( "CENTRILOBULARNODULE" );
      LungTypeStringVector.push_back( "MOSAICING" );
      LungTypeStringVector.push_back( "EXPIRATORYMALACIA" );
      LungTypeStringVector.push_back( "SABERSHEATH" );
      LungTypeStringVector.push_back( "OUTPOUCHING" );
      LungTypeStringVector.push_back( "MUCOIDMATERIAL" );
      LungTypeStringVector.push_back( "PATCHYGASTRAPPING" );
      LungTypeStringVector.push_back( "DIFFUSEGASTRAPPING" );
      LungTypeStringVector.push_back( "LINEARSCAR" );
      LungTypeStringVector.push_back( "CYST" );
      LungTypeStringVector.push_back( "ATELECTASIS" );
      LungTypeStringVector.push_back( "HONEYCOMBING" );
      LungTypeStringVector.push_back( "AIRWAYGENERATION0" );
      LungTypeStringVector.push_back( "AIRWAYGENERATION1" );
      LungTypeStringVector.push_back( "AIRWAYGENERATION2" );
      LungTypeStringVector.push_back( "AIRWAYGENERATION3" );
      LungTypeStringVector.push_back( "AIRWAYGENERATION4" );
      LungTypeStringVector.push_back( "AIRWAYGENERATION5" );
      LungTypeStringVector.push_back( "AIRWAYGENERATION6" );
      LungTypeStringVector.push_back( "AIRWAYGENERATION7" );
      LungTypeStringVector.push_back( "AIRWAYGENERATION8" );
    };

  unsigned char GetNumberOfEnumeratedLungRegions()
    {
      return m_NumberOfEnumeratedLungRegions;
    };

  unsigned char GetNumberOfEnumeratedLungTypes()
    {
      return m_NumberOfEnumeratedLungTypes;
    };

  /** This method checks if the lung region 'subordinate' is within
   *  the lung region 'superior'. It assumes that all lung regions are
   *  within the WHOLELUNG lung region. */
  bool CheckSubordinateSuperiorLungRegionRelationship( unsigned char subordinate, unsigned char superior )
    {
      if ( subordinate == superior || superior == static_cast< unsigned char >( WHOLELUNG ) )
        {
        return true;
        }

      unsigned char subordinateTemp = subordinate;

      while ( subordinateTemp != static_cast< unsigned char >( WHOLELUNG ) && 
              subordinateTemp != static_cast< unsigned char >( UNDEFINEDREGION ) )
        {
        if ( LungRegionHierarchyMap[subordinateTemp] == superior )
          {
          return true;
          }
        else
          {
          subordinateTemp = LungRegionHierarchyMap[subordinateTemp];
          }
        }

      return false;
    }

  /** Given an unsigned short value, this method will compute the
   *  8-bit region value corresponding to the input */
  unsigned char GetLungRegionFromValue( unsigned short value )
    {
      unsigned char regionValue = 0;

      for ( int i=15; i>=0; i-- )
        {
        int power = static_cast< int >( pow( 2, i ) );
        
        if ( power <= value )
          {
          if ( i < 8 )
            {
            regionValue += power;
            }
      
          value = value % power;
          }
        }
      
      return regionValue;
    };

  /** Given an unsigned short value, this method will compute the
   *  8-bit type value corresponding to the input */
  unsigned char GetLungTypeFromValue( unsigned short value )
    {
      unsigned char typeValue = 0;

      for ( int i=15; i>=0; i-- )
        {
        int power = static_cast< int >( pow( 2, i ) );
        
        if ( power <= value )
          {
          if ( i >= 8 )
            {
            typeValue += static_cast< unsigned char >( pow( 2, i-8 ) );
            }
          
          value = value % power;
          }
        }

      return typeValue;
    };

  /** Given an unsigned short value, this method will return the
   *  string name of the corresponding lung region */
  std::string GetLungRegionStringFromValue( unsigned short value )
    {
      unsigned char regionValue = 0;

      for ( int i=15; i>=0; i-- )
        {
        int power = static_cast< int >( pow( 2, i ) );
        
        if ( power <= value )
          {
          if ( i < 8 )
            {
            regionValue += power;
            }
      
          value = value % power;
          }
        }
      
      return LungRegionStringVector[static_cast< int >( regionValue )];
    };

  /** Given an unsigned short value, this method will return the
   *  string name of the corresponding lung type */
  std::string GetLungTypeStringFromValue( unsigned short value )
    {
      unsigned char typeValue = 0;

      for ( int i=15; i>=0; i-- )
        {
        int power = static_cast< int >( pow( 2, i ) );
        
        if ( power <= value )
          {
          if ( i >= 8 )
            {
            typeValue += static_cast< unsigned char >( pow( 2, i-8 ) );
            }
          
          value = value % power;
          }
        }

      return LungTypeStringVector[static_cast< int >( typeValue )];
    };

  unsigned short GetValueFromLungRegionAndType( unsigned char region, unsigned char type )
    {
      //
      // Get the binary representation of the region to set
      //
      int regionValue = static_cast< int >( region );

      int regionPlaces[8];
      for ( int i=0; i<8; i++ )
        {
        regionPlaces[i] = 0;
        }

      for ( int i=7; i>=0; i-- )
        {
        int power = static_cast< int >( pow( 2, i ) );

        if ( power <= regionValue )
          {
          regionPlaces[i] = 1;
          
          regionValue = regionValue % power;
          }
        }
 
      //
      // Get the binary representation of the type to set
      //
      int typeValue = static_cast< int >( type );

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
      unsigned short combinedValue = 0;

      for ( int i=0; i<16; i++ )
        {
        if ( i < 8 )
          {
          combinedValue += static_cast< unsigned short >( regionPlaces[i] )*static_cast< unsigned short >( pow( 2, i ) );
          }
        else
          {
          combinedValue += static_cast< unsigned short >( typePlaces[i-8] )*static_cast< unsigned short >( pow( 2, i ) );
          }
        }

      return combinedValue;
    };

  /** Given a string identifying one of the enumerated lung regions,
   * this method will return the unsigned char equivalent. If no match
   * is found, the method will retune UNDEFINEDREGION */
  unsigned char GetLungRegionValueFromString( std::string regionString )
    {
      for ( int i=0; i<m_NumberOfEnumeratedLungRegions; i++ )
        {
        if ( !regionString.compare( LungRegionStringVector[i] ) )
          {
          return LungRegionVector[i];
          }
        }
      
      return static_cast< unsigned char >( UNDEFINEDREGION );
    }

  /** Given a string identifying one of the enumerated lung types,
   * this method will return the unsigned char equivalent. If no match
   * is found, the method will retune UNDEFINEDTYPE */
  unsigned char GetLungTypeValueFromString( std::string typeString )
    {
      for ( int i=0; i<m_NumberOfEnumeratedLungTypes; i++ )
        {
        if ( !typeString.compare( LungTypeStringVector[i] ) )
          {
          return LungTypeVector[i];
          }
        }
      
      return static_cast< unsigned char >( UNDEFINEDTYPE );
    }

public:
  std::map< unsigned char, unsigned char >  LungRegionHierarchyMap;
  std::vector< unsigned char >              LungRegionVector;
  std::vector< unsigned char >              LungTypeVector;
  std::vector< std::string >                LungRegionStringVector;
  std::vector< std::string >                LungTypeStringVector;

private:
  unsigned char m_NumberOfEnumeratedLungRegions;
  unsigned char m_NumberOfEnumeratedLungTypes;
};


#endif
