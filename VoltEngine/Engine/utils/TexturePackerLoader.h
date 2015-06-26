#ifndef TEXTUREPACKERLOADER_H
#define TEXTUREPACKERLOADER_H

#include<QObject>
#include<QJsonDocument>
#include<QRect>
#include<QMap>
#include"LoaderInterface.h"

class QString;
/**
*   @brief Struct containing the required information for a loaded frame.
*/
struct TexturePackerImage{
    int   textureId;//used to identify in which texture to find this.
    bool  rotated, trimmed;
    QString name;
    QRect frame;
    QRect sprite_src_size;
    int   sourceSize[2];
    float pivot[2];
};
typedef TexturePackerImage TP_Image;

/**
*   Useful meta information.
*/
struct TexturePackerMetadata{
    QString image_name;
    QString image_path;
    QString image_format;
    int size[2];
    float scale;
    QMap<QString,TP_Image*> m_images;
};

typedef TexturePackerMetadata TP_Document;


/**
*   @brief A loader for TexturePacker Json file.
*   @note TexturePacker can export spritesheets as a .json file structure
*         for frames.
*
*   TODO:
*   - Add checks for properties needed of the file.
*   - Add some kind of cache for loaded textures.
*   - Add simple image provider for using this images anywhere in the qml environment.
*     which will required a custom qml item, in which the texture will be rendered.
*     simply by enabling this behavior into the ImageRender item.
*/
class TexturePackerLoader: public QObject
{
    Q_OBJECT
public:
    virtual ~TexturePackerLoader();

    /**
    *   @brief returns the loaded TP_Document, if chached returns the cached one.
    */
    TP_Document* getTPDocument(const QString& name);

    static TexturePackerLoader* getInstance();

private:
    TexturePackerLoader(QObject* parent = nullptr);

    static TexturePackerLoader* mInstance;
    QMap<QString,TP_Document*>  mCachedDocs;
};

#endif // TEXTUREPACKERLOADER_H
