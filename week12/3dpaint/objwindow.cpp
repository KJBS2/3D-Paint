#include "objwindow.h"
#include<QFile>
#include<QString>

ObjWindow::ObjWindow(QString _file)
{
    file=_file;
    aVertex.clear();
    aNormal.clear();
    aFace.clear();
    size=0;


    parse();
}
void ObjWindow::parse()
{
    QFile target(file);

    if(target.open(QIODevice::ReadOnly))
    {
        QTextStream in(&target);
        while(!in.atEnd())
        {
            QString input = in.readLine();
            char* line =(char*)input.toStdString().c_str();

            int i;

            if(!strncmp(line,"v ",2))
            {
                Vector3 v;
                sscanf(line+2,"%f%f%f",&v.x,&v.y,&v.z);
                aVertex.push_back(v);
                if(v.getNorm()>size)size=v.getNorm();
            }
            else if(!strncmp(line,"vn ",3))
            {
                Vector3 v;
                sscanf(line+2,"%f%f%f",&v.x,&v.y,&v.z);
                aNormal.push_back(-v);
            }
            else if(!strncmp(line,"f ",2))
            {
                int t;
                Vector3 n;
                aFace.push_back(Face());
                char *pt=line+2;
                while(1)
                {
                    sscanf(pt,"%d",&t);
                    aFace.back().vertexNo.push_back(t);
                    if(strstr(pt,"/"))
                    {
                        pt=strstr(pt,"/")+1;
                        if(strstr(pt,"/")&&strstr(pt,"/")<strstr(pt," "))
                        {
                            pt=strstr(pt,"/")+1;
                            sscanf(pt,"%d",&t);
                            n=aNormal[t-1];
                        }
                    }
                    aFace.back().normal.push_back(n);
                    if(strstr(pt," "))pt=strstr(pt," ")+1;
                    else break;
                    for(i='0';i<='9';++i)
                    {
                        char digit[2];
                        digit[0]=i;
                        digit[1]=0;
                        if(strstr(pt,digit))break;
                    }
                    if(i>'9')break;
                }
            }
        }
    }
}
