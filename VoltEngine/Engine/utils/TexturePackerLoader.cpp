#include "TexturePackerLoader.h"
#include "utils/Util.h"

#include<QFile>
#include<QFileInfo>
#include<QJsonArray>

TexturePackerLoader* TexturePackerLoader::mInstance = nullptr;

TexturePackerLoader* TexturePackerLoader::getInstance(){
    if(!mInstance){
        mInstance = new TexturePackerLoader;
    }
    return mInstance;
}

TexturePackerLoader::TexturePackerLoader(QObject* parent)
{
}

TexturePackerLoader::~TexturePackerLoader(){

}

TP_Document* TexturePackerLoader::getTPDocument(const QString &name)
{
    TP_Document* c_tp_doc = mCachedDocs.value(name);
    if(c_tp_doc){
        return c_tp_doc;
    }

    QFileInfo f_info(name);
    if(!f_info.exists()){
        return nullptr;
    }

    QByteArray b_arr = Util::readFileAsBytes(const_cast<QString&>(name));

    QJsonParseError parse_error;
    QJsonDocument m_json = QJsonDocument::fromJson(b_arr,&parse_error);

    TP_Document* tp_doc = nullptr;

    QJsonObject object = m_json.object();
    if(!object.isEmpty())
    {
        tp_doc = new TP_Document;

        QJsonObject meta =  object.value("meta").toObject();

        QJsonObject  sizetmp;
        if(!meta.empty()){
            tp_doc->image_name = meta["image"].toString();
            tp_doc->image_path =  f_info.absolutePath().append("/").append(tp_doc->image_name);
            tp_doc->image_format = meta["format"].toString();

            sizetmp = meta["size"].toObject();
            tp_doc->size[0] = sizetmp["w"].toInt();
            tp_doc->size[1] = sizetmp["h"].toInt();

            qDebug() << " meta size " << tp_doc->size[0] << "x" << tp_doc->size[1];

            tp_doc->scale = meta["scale"].toDouble();
        }

         QJsonArray frames = object.value("frames").toArray();

         TP_Image* t_img = nullptr;
         auto b = frames.begin();
         QJsonObject f;
         QJsonObject frame,pivot,spriteSourceSize,sourceSize;

         for(;b != frames.end();b++)
         {
            f = (*b).toObject();
            qDebug() << " F " << f;

            t_img = new TP_Image;
            t_img->name = f["filename"].toString();
            t_img->rotated = f["rotated"].toBool();
            t_img->trimmed = f["trimmed"].toBool();

            frame = f["frame"].toObject();
            t_img->frame.setRect(frame["x"].toInt(),frame["y"].toInt(),frame["w"].toInt(),frame["h"].toInt());

            qDebug() << "IMG:frame " << t_img->frame;

            spriteSourceSize = f["spriteSourceSize"].toObject();
            t_img->sprite_src_size.setRect(spriteSourceSize["x"].toInt(),
                                 spriteSourceSize["y"].toInt(),
                                 spriteSourceSize["w"].toInt(),
                                 spriteSourceSize["h"].toInt());

            sourceSize = f["sourceSize"].toObject();
            t_img->sourceSize[0] = sourceSize["w"].toInt();
            t_img->sourceSize[1] = sourceSize["h"].toInt();

            pivot = f["pivot"].toObject();
            t_img->pivot[0] = pivot["w"].toDouble();
            t_img->pivot[1] = pivot["h"].toDouble();

            tp_doc->m_images.insert(t_img->name,t_img);
            tp_doc->m_images.insert(t_img->name.split(".").at(0),t_img);

           // qDebug() << " Frame "  << f.toVariantMap();
         }
         mCachedDocs.insert(name, tp_doc);

//         QMap<QString,TP_Image*>::Iterator it = tp_doc->m_images.begin();
//         for(; it != tp_doc->m_images.end();it++){
//             TP_Image* img = static_cast<TP_Image*>(it.value());
//             qDebug() << " Key " << it.key();
//             qDebug() << " ValueName "  <<img->name;
//             qDebug() << " ValueFrame "  <<img->frame;
//             qDebug() << " ValueSpriteSourceSize "  <<img->sprite_src_size;
//             qDebug() << " ValueSourceSize[1] "  <<img->sourceSize[0];
//             qDebug() << " ValueSourceSize[1] "  <<img->sourceSize[1];
//         }
    }
    return tp_doc;
}


