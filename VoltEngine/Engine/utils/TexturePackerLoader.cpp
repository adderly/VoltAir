#include "TexturePackerLoader.h"
#include "utils/Util.h"

TexturePackerLoader::TexturePackerLoader(QObject* parent ):
    QObject(parent)
{

}

TexturePackerLoader::~TexturePackerLoader(){

}

void TexturePackerLoader::setFilename(QString value){
    m_filename = value;
    emit filenameChanged();
}

uint TexturePackerLoader::errorCode(){
    return m_error_code;
}

bool TexturePackerLoader::isLoaded(){
    return m_error_code == no_error;
}

bool TexturePackerLoader::load()
{
    if(m_filename.isEmpty()){
        m_error_code = file_error;
        m_error_string = " Empty filename. ";
    }else{
        QFileInfo f_info(m_filename);
        if(f_info.exists())
        {
            m_json = QJsonDocument::fromBinaryData(Util::readFileAsBytes(m_filename));
            if(!m_json.isEmpty() && !m_json.isNull()){
                readFrames();
            }else{
                m_error_code = file_error;
                m_error_string = " Json file could not be read. ";
            }
        }else{
            m_error_code = file_error;
            m_error_string = " File not exists. ";
        }
    }
    return m_error_code == no_error;
}

bool TexturePackerLoader::load(QString filename)
{

}

void TexturePackerLoader::readFrames()
{
    QJsonObject object = m_json.object();
    if(!object.isEmpty())
    {
         QJsonObject frames = object.value("frames").toObject();

         auto b = frames.begin();
         for(;b != frames.end();b++)
         {
            QJsonObject frame = b.value().toObject();
            qDebug() << frame.toVariantMap();
         }
    }
}

