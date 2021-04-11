#include <QApplication>
#include <QColor>
#include <QGridLayout>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include "SurfaceBox.h"
#include "ThreadPool.h"
#include <vector>
typedef unsigned char byte;
typedef unsigned int uint;
QImage toImage(Bitmap *f) {
  QImage temp(reinterpret_cast<const unsigned char *>(f->getPointAdress(0, 0)),
              f->width, f->height, QImage::Format_ARGB32);
  return temp;
}
// QImage toImage(Bitmap *f){
//     QImage temp(f->width,f->height,QImage::Format_RGB888);
//     for(int i=0;i<f->width;i++)
//         for(int j=0;j<f->height;j++){
//             bgra pt=f->getPoint(i,j);
//             temp.setPixelColor(i,j,QColor(pt.R,pt.G,pt.B,pt.A));
//         }
//     return temp;
// }



//Surface fromimage(QImage *s){

//    byte copiedData[s->width()*s->height()*sizeof(QColor)];
//    memcpy(copiedData, s->bits(), s->width()*s->height()*sizeof(QColor));
//}
Surface fromImage(QImage *s) {
  Surface temp(s->width(), s->height());
  for (int i = 0; i < s->width(); i++)
    for (int j = 0; j < s->height(); j++) {
      auto t = s->pixelColor(i, j);
      *temp.getPointAdress(i, j) = bgra(t.red(), t.green(), t.blue());
    }
  return temp;
}
#include <QPainter>
class FragTreeViewer : public QWidget {
  QGridLayout m_layout{this};
  QScrollArea m_area;
  QLabel m_imageLabel, m_scaleLabel;
  QPushButton m_zoomOut{"Zoom Out"}, m_zoomIn{"Zoom In"};
  double m_scaleFactor = 1.0;

 public:
  void setImage(const QImage &img) {
    m_scaleFactor = 1.0;
    m_imageLabel.setPixmap(QPixmap::fromImage(img));
    scaleImage(1.0);
  }
  FragTreeViewer() {
    m_layout.addWidget(&m_area, 0, 0, 1, 3);
    m_layout.addWidget(&m_zoomOut, 1, 0);
    m_layout.addWidget(&m_scaleLabel, 1, 1);
    m_layout.addWidget(&m_zoomIn, 1, 2);
    m_area.setWidget(&m_imageLabel);
    m_imageLabel.setScaledContents(true);
    connect(&m_zoomIn, &QPushButton::clicked, [this] { scaleImage(1.1); });
    connect(&m_zoomOut, &QPushButton::clicked,
            [this] { scaleImage(1.0 / 1.1); });
  }

  void scaleImage(double factor) {
    m_scaleFactor *= factor;
    m_scaleLabel.setText(
        QStringLiteral("%1%").arg(m_scaleFactor * 100, 0, 'f', 1));
    QSize size = m_imageLabel.pixmap().size() * m_scaleFactor;
    m_imageLabel.resize(size);
  }
};
#include <iostream>
int main(int argc, char *argv[]) {
    ThreadPool pool(8);
    std::vector< std::future<int> > results;
    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i*i;
            })
            );
    }
//    for(auto && result: results)
//        std::cout << result.get() << ' ';
//    std::cout << std::endl;
  QApplication a(argc, argv);
  FragTreeViewer viewer;
  FragTreeViewer viewer2;
  QImage l("C:\\Users\\aleks\\Pictures\\photo_2021-03-16_20-02-37.jpg");
  QImage ll("C:\\Users\\aleks\\Pictures\\photo_2021-03-16_20-03-56.jpg");
  Surface image = fromImage(&l);
  Surface image1 = fromImage(&ll);

 // Surface end2(800,600);
////  end2.clearCheckboard();
 // QImage end(reinterpret_cast<byte*>(end2.getPointAdress(0,0)),800,600,QImage::Format_ARGB32);
//  SurfaceBox tf;
//  tf.show();
 // image1.copySurface(&image,QPoint(11,11));
  Surface *image11 = (Surface *)image.createSubBitmap(0, 10, 100, 100);
  Surface image2(500, 500);
  UPO_Invert().apply(image11);
  Surface image3(500, 500);
  image2.superSamplingFitSurface(&image, image2.bounds);

  image3.superSamplingFitSurface(&image1, image3.bounds);
  SurfaceBox p;
  p.setSurface(image11);
  p.renderTo(&image2);
  p.show();
  QImage t = toImage(&image2);
  viewer.setImage(t);
  viewer.show();
  //PBO_Multiply().apply(&image3, &image2);
  //  PBO_ColorBurn().apply(&image3,&image2);
  // PBO_Negation().apply(&image3,&image2);
  // PBO_Normal().apply(&image3,&image2);
 // UPO_Invert().apply(&end2);
  QImage te = toImage(&image3);
  viewer2.setImage(te);
  viewer2.show();
//  UPO_Invert().apply(&end2);

  viewer2.resize(200, 200);
  //  qDebug()<<s.getRow(200)[100*sizeof(bgra)+1];
  // 3=255 2=255 1=123 0=0 Littl endian

  // Widget w;
  //  w.show();
  return a.exec();
}
