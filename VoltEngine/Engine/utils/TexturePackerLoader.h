#ifndef TEXTUREPACKERLOADER_H
#define TEXTUREPACKERLOADER_H

#include<QObject>
#include<QFile>
#include<QFileInfo>
#include<QJsonDocument>
#include"LoaderInterface.h"

/**
*   @brief Struct containing the required information for a loaded frame.
*/
struct TexturePackerImage{
    int textureId;//used to identify in which texture to find this.
    bool rotated, trimmed;
    int frame[4];
    int image_rect[4];
    float pivot[2];
    QString name;
};

typedef TexturePackerImage TP_Image;
/**
*   @brief A loader for TexturePacker Json file.
*   @note TexturePacker can export spritesheets as a .json file structure
*         for frames.
*
*   TODO:
*   - Complete parser of the json file.
*   - Add some kind of cache for loaded textures.
*   - Add simple image provider for using this images anywhere in the qml environment.
*     which will required a custom qml item, in which the texture will be rendered.
*     simply by enabling this behavior into the ImageRender item.
*/
class TexturePackerLoader: public QObject
{
    Q_OBJECT
public:
    explicit TexturePackerLoader(QObject* parent = nullptr);
    virtual ~TexturePackerLoader();

    /**
    *   @brief
    */
    void setFilename(QString value);

    /**
    *   @brief
    */
    QString filename()  {  return m_filename;  }

    /**
    *   @brief Loads the specified json file.
    */
    bool load();

    /**
    *   @brief
    */
    bool load(QString filename);

    /**
    *   @brief Return the error code, if any after loading the file.
    */
    uint errorCode();

    /**
    *   @brief Returns whether the file is loaded or not.
    */
    bool isLoaded();

    /**
    *   @brief Return the frame's container.
    */
    std::vector<TP_Image*>& getFrames() { return m_loaded_frames;  }

signals:
    void filenameChanged();

private:

    /**
    *   @brief Internal function where the frames are loaded.
    */
    void readFrames();

    bool m_ok;
    uint m_error_code = stateless_error;
    QString m_error_string;
    QString m_filename;
    QJsonDocument m_json;
    std::vector<TP_Image*> m_loaded_frames;
};

#endif // TEXTUREPACKERLOADER_H
