
#ifndef __OSG_2_PRC_H__
#define __OSG_2_PRC_H__ 1

#include <osg\NodeVisitor>

#ifdef PRC_USE_ASYMPTOTE
#  include <oPRCFile.h>
#endif

#include <vector>
#include <map>


namespace osg {
    class MatrixTransform;
    class Geode;
    class Geometry;
    class StateSet;
    class Material;
    class DrawArrays;
    class DrawArrayLengths;
    class DrawElementsUByte;
    class DrawElementsUShort;
    class DrawElementsUInt;
}


/** \class OSG2PRC OSG2PRC.h
\brief OSG NodeVisitor that interfaces with libPRC.
\details TBD
*/
class OSG2PRC : public osg::NodeVisitor
{
public:
    OSG2PRC();
#ifdef PRC_USE_ASYMPTOTE
    OSG2PRC( oPRCFile* prcFile );
#endif
    virtual ~OSG2PRC();

    virtual void apply( osg::Node& node );
    virtual void apply( osg::Transform& trans );
    virtual void apply( osg::Geode& geode );

protected:
    void apply( const osg::StateSet* stateSet );

    void processNewNode( const std::string& name );
    void processTransformNode( const std::string& name, const osg::Matrix& matrix );
    void finishNode();

    void apply( const osg::Geometry* geom );
    static void apply( const osg::PrimitiveSet* ps, PRC3DTess* tess, PRCTessFace *tessFace, const osg::Geometry* geom );

    static void processDrawArrays( const osg::DrawArrays* da, PRC3DTess* tess, PRCTessFace *tessFace, const osg::Geometry* geom );
    static void processDrawArrayLengths( const osg::DrawArrayLengths* dal, PRC3DTess* tess, PRCTessFace *tessFace, const osg::Geometry* geom );
    static void processDrawElements( const osg::DrawElements* de, PRC3DTess* tess, PRCTessFace *tessFace, const osg::Geometry* geom );

    static void processIndex( const unsigned int index, PRC3DTess* tess, PRCTessFace *tessFace,
                           const bool hasNormals, const bool hasTexCoords, const osg::Geometry* geom );

    typedef std::vector< osg::ref_ptr< const osg::Material > > MaterialStack;
    MaterialStack _materialStack;
    void pushMaterial();
    bool popMaterial();
    void setMaterial( const osg::Material* mat );
    const osg::Material* getMaterial() const;
    void addDefaultMaterial();

    typedef std::vector< float > AlphaStack;
    AlphaStack _nodeAlphaStack;
    void pushNodeAlpha();
    bool popNodeAlpha();
    void setNodeAlpha( const float alpha );
    float getNodeAlpha() const;
    static bool checkNodeAlpha( float& alpha, const osg::Node* node );

    typedef std::pair< const osg::Material*, float > StyleAlphaKey;
    typedef std::map< StyleAlphaKey, uint32_t > StyleAlphaMap;
    StyleAlphaMap _styleAlphaMap;
    uint32_t getStyle( const osg::Material* mat, const float alpha );

    void processNodeAlpha( const osg::Node* node );


    PRC3DTess* createTess( const osg::Geometry* geom );

protected:
#ifdef PRC_USE_ASYMPTOTE

    static RGBAColour colorToPRC( const osg::Vec4& osgColor )
    {
        return( RGBAColour( osgColor[ 0 ],
            osgColor[ 1 ],osgColor[ 2 ],osgColor[ 3 ] ) );
    }

    oPRCFile* _prcFile;

#endif

};


// __OSG_2_PRC_H__
#endif
