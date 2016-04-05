#include "parser.h"
#include "mesh.h"
void
Parser::setMesh(Mesh *mesh)
{
    this->mesh = mesh;
}

void
Parser::setPath(char *path)
{
    this->path = path;
}

void
Parser::parse()
{
    int i;
    FILE*fp=fopen(path,"r");
    if(fp==NULL)return;
//    initialize();
    mesh->aVertex.clear();
    mesh->aFace.clear();
    mesh->size=0;
    while(!feof(fp))
    {
        char line[1024];
        fgets(line,1024,fp);
        if(!strncmp(line,"v ",2))
        {
            Vector3 v;
            sscanf(line+2,"%f%f%f",&v.x,&v.y,&v.z);
            mesh->aVertex.push_back(v);
            if(v.getNorm()>mesh->size)mesh->size=v.getNorm();
        }
        else if(!strncmp(line,"vn ",3))
        {
            Vector3 v;
            sscanf(line+2,"%f%f%f",&v.x,&v.y,&v.z);
            mesh->aNormal.push_back(-v);
        }
        else if(!strncmp(line,"f ",2))
        {
            int t;
            Vector3 n;
            mesh->aFace.push_back(Face());
            char *pt=line+2;
            while(1)
            {
                sscanf(pt,"%d",&t);
                mesh->aFace.back().vertexNo.push_back(t);
                if(strstr(pt,"/"))
                {
                    pt=strstr(pt,"/")+1;
                    if(strstr(pt,"/")&&strstr(pt,"/")<strstr(pt," "))
                    {
                        pt=strstr(pt,"/")+1;
                        sscanf(pt,"%d",&t);
                        n=mesh->aNormal[t-1];
                    }
                }
                mesh->aFace.back().normal.push_back(n);
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
    fclose(fp);
}

int
Parser::findTokenFormat(char *token)
{
    int cntSlashes = 0;
    int len = strlen(token);
    int ret = 0;

    bool secondElementExist = false;

    for(int i=0;i<len;i++ ) if(token[i] == '/'){
        cntSlashes++;
        if( cntSlashes == 1     &&
            i+1<len             &&
            token[i+1]!='/' )
            secondElementExist = true;
    }

    //4cases
    // 0 : v
    if(cntSlashes == 0) ret = 0;
    // 1 : v/vt
    if(cntSlashes == 1) ret = 1;
    // 2 : v/vt/vn
    if(cntSlashes == 2 && secondElementExist) ret = 2;
    // 3 : v//vn
    if(cntSlashes == 2 && !secondElementExist) ret = 3;

    return ret;
}
/*
void
Parser::toTriangle(void)
{
    for(POLYGON& polygon : mesh->polygon)
    {
        //dividing : one polygon-> several triangles
        TRIANGLE t;

        for(int i=0;i+1<polygon.vIndex.size();i++)
        {
            t.v[0] = mesh->v[polygon.vIndex[0] - 1];
            t.v[1] = mesh->v[polygon.vIndex[i] - 1];
            t.v[2] = mesh->v[polygon.vIndex[i+1] - 1];

            if(!polygon.vtIndex.empty())
            {
                t.vt[0] = mesh->vt[polygon.vtIndex[0]-1];
                t.vt[1] = mesh->vt[polygon.vtIndex[i]-1];
                t.vt[2] = mesh->vt[polygon.vtIndex[i+1]-1];
            }else
            {
                /* In later. this place should be filled... *-/
            }

            if(!polygon.vnIndex.empty())
            {
                t.vn[0] = mesh->vn[polygon.vnIndex[0]-1];
                t.vn[1] = mesh->vn[polygon.vnIndex[i]-1];
                t.vn[2] = mesh->vn[polygon.vnIndex[i+1]-1];
            }else
            {
                /* In later. this place should be filled... *-/
            }

        }

        mesh->triangle.push_back(t);
    }
}
*/
