#include "openglwidget.h"
#include <QApplication>
#include <QScreen>
OpenGLWidget::OpenGLWidget(QWidget *parent) :QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{


    vboIndices=NULL;
    vboTextureCoordinate=NULL;
    vertexShader=NULL;
    fragmentShader=NULL;
    shaderProgram=NULL;
    imgOrig=NULL;

}
OpenGLWidget::~OpenGLWidget(){
    destroyVBOs();
    destroyShaders();
    if(imgOrig) delete imgOrig;
}
void OpenGLWidget::toggleBackgroundColor( bool changeBColor){
    makeCurrent();
    if(changeBColor)
        glClearColor(0,0,0,1);
    else
        glClearColor(1,1,1,1);

    update();
}

void OpenGLWidget::slotFilter(int filter){
    update();
}
void OpenGLWidget::destroyShaders(){
    if(vertexShader)delete vertexShader;
    vertexShader=NULL;
    if(fragmentShader)delete fragmentShader;
    fragmentShader=NULL;
    if(shaderProgram){
        shaderProgram->release();
         delete shaderProgram;
         shaderProgram=NULL;
     }
 
}
void OpenGLWidget::destroyVBOs(){
    if(vboTextureCoordinate){
        vboTextureCoordinate->release();
        delete vboTextureCoordinate;
        vboTextureCoordinate=NULL;
    }

    if(vboIndices){
        vboIndices->release();
        delete vboIndices;
        vboIndices=NULL;
    }
}
void OpenGLWidget::createShaders(){
    destroyShaders();
    QString vertexShaderFile="shader.vert";
    QString fragmentShaderFile="shader.frag";

    shaderProgram = new QOpenGLShaderProgram;
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,FileOperationsFromResource::readFile(vertexShaderFile));
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,FileOperationsFromResource::readFile(fragmentShaderFile));
    if(!shaderProgram->link()){
        qWarning() << shaderProgram->log() << endl;
    }

    qWarning()<< "==========================================" <<endl;


}


void OpenGLWidget::createVBOs(){
    destroyVBOs();//destroy previous (GPU allocated) VBOs
    //define coordinate texture=========================================
    QVector2D *qVector2DTextureCoordinate=new QVector2D[4];
    qVector2DTextureCoordinate[3]= QVector2D(0,0);
    qVector2DTextureCoordinate[2]=QVector2D(1,0);
    qVector2DTextureCoordinate[1]=QVector2D(1,1);
    qVector2DTextureCoordinate[0]=QVector2D(0,1);
    //end===============================================================

    //transfer coordenate  texture to gpu===============================
    vboTextureCoordinate = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboTextureCoordinate->create();
    vboTextureCoordinate->bind();
    vboTextureCoordinate->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboTextureCoordinate->allocate(qVector2DTextureCoordinate , 4*sizeof(QVector2D));
    //end===============================================================

    //define face to form one square====================================
    unsigned int  *indices = new unsigned int[2*3];
    indices[0]=0; indices[1]=1; indices[2]=2;
    indices[3]=2; indices[4]=3; indices[5]=0;
    //end===============================================================
    //transfer face to gpu==============================================
    vboIndices =new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    vboIndices ->create();
    vboIndices ->bind();
    vboIndices ->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboIndices ->allocate(indices , 2 * 3 *sizeof(unsigned int));
    //end===============================================================
    delete [] indices;
    delete qVector2DTextureCoordinate;
}



void OpenGLWidget::slotSendImage(){

}


void OpenGLWidget::slotRecvImage(QImage *img){
    if(imgOrig) delete imgOrig;
    imgOrig=img;
    update();
}


void OpenGLWidget::initializeGL(){
    /*
    qWarning() << "ola";
    this->createVBOs();
    this->createShaders();
    qWarning() << "end";
    initializeOpenGLFunctions();

    paintGL();
    */
    
    //qglClearColor((QColor::fromCmykF(0.40, 0.0, 1.0, 0.0)).dark());

    
    
}
void OpenGLWidget::resizeGL(int w, int h){

    glViewport(0, 0,w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}
void OpenGLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    paintImg(imgOrig,NULL,0,0);
}

void OpenGLWidget::paintImg(QImage *img,QImage *img2,int filter,int passFilter){
    /*GLuint zoomicon;
	QImage t;
	QImage b;

	glEnable(GL_TEXTURE_2D);
	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_ALPHA_TEST);
	
	b.load("teste.png", "PNG");
	QImage fixedImage( b.width(), b.height(), QImage::Format_ARGB32);
	QPainter painter(&fixedImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(fixedImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawImage( 0, 0, b);
	painter.end();
	
	t = QGLWidget::convertToGLFormat( fixedImage );
	glGenTextures( 1, &zoomicon );
	glBindTexture( GL_TEXTURE_2D, zoomicon );
	glTexImage2D( GL_TEXTURE_2D, 0, 4, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);



    


    
    

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glBindTexture(GL_TEXTURE_2D, zoomicon);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-1.0f, 1.0f);
	glEnd();
    */
	glDisable(GL_TEXTURE_2D); 

}
