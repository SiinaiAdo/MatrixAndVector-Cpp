#ifndef MATRIX_AND_VECTOR
#define MATRIX_AND_VECTOR

#include <iostream>
#include <cmath>

class Vector3f{
    friend Vector3f operator+(Vector3f lv,const Vector3f& rv);
    friend Vector3f operator-(Vector3f lv,const Vector3f& rv);
    private:
        float x,y,z;
    public:
        Vector3f();
        Vector3f(float,float,float);
        Vector3f& operator+=(const Vector3f&);
        Vector3f& operator-=(const Vector3f&);
        Vector3f operator*(float)const;
        float dot(const Vector3f&)const;
        Vector3f cross(const Vector3f&)const;
        float length()const;
        Vector3f normalized()const;
        const float& operator()(int)const;
        float& operator()(int);
        void show()const;
};


class Matrix4f{
    friend Matrix4f operator+(Matrix4f,const Matrix4f&);
    friend Matrix4f operator-(Matrix4f,const Matrix4f&);
private:
    float m[16];
public:
    Matrix4f();
    Matrix4f(const float (&values)[4][4]);
    float& operator()(int i, int j);
    const float& operator()(int i, int j)const;
    void show() const;
    Matrix4f& operator+=(const Matrix4f& rm);
    Matrix4f& operator-=(const Matrix4f& rm);
    Matrix4f operator*(const Matrix4f& rm) const;
    static Matrix4f translation(float tx, float ty, float tz);
    static Matrix4f scale(float sx, float sy, float sz);
    static Matrix4f rotationX(float angleRad);
    static Matrix4f rotationY(float angleRad);
    static Matrix4f rotationZ(float angleRad);
    Vector3f operator*(const Vector3f&)const;
};


inline Matrix4f::Matrix4f(){
    for(int i=0;i<16;i++){
        if(i%5 == 0){
            m[i]=1;
        }
        else{
            m[i]=0;
        } 
    }
}
inline Matrix4f::Matrix4f(const float (&values)[4][4]){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            m[i*4+j]=values[i][j];
        }
    }
}
inline float& Matrix4f::operator()(int i, int j){
    return m[i*4+j];
}
inline const float& Matrix4f::operator()(int i, int j)const {

    return m[i*4+j];
}
inline Matrix4f& Matrix4f::operator+=(const Matrix4f& rm){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            (*this)(i,j)+=rm(i,j);
        }
    }
    return *this;
}
inline Matrix4f& Matrix4f::operator-=(const Matrix4f& rm){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            (*this)(i,j)-=rm(i,j);
        }
    }
    return *this;
}
inline Matrix4f operator+(Matrix4f lm,const Matrix4f& rm){
    return lm+=rm;
}
inline Matrix4f operator-(Matrix4f lm,const Matrix4f& rm){
    return lm-=rm;
}
inline void Matrix4f::show() const{
    for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                std::cout<<m[i*4+j]<<' ';
            }
            std::cout<<std::endl;
        }
}
inline Matrix4f Matrix4f::translation(float tx, float ty, float tz){
    Matrix4f ans;
    ans(0,3)=tx;
    ans(1,3)=ty;
    ans(2,3)=tz;
    return ans;
}
inline Matrix4f Matrix4f::scale(float sx, float sy, float sz){
    Matrix4f ans;
    ans(0,0)=sx;
    ans(1,1)=sy;
    ans(2,2)=sz;
    return ans;
}
inline Matrix4f Matrix4f::rotationX(float angleRad){
    Matrix4f x;
    x(1,1)=std::cos(angleRad);
    x(1,2)=-std::sin(angleRad);
    x(2,1)=std::sin(angleRad);
    x(2,2)=std::cos(angleRad);
    return x;
}
inline Matrix4f Matrix4f::rotationY(float angleRad){
    Matrix4f y;
    y(0,0)=std::cos(angleRad);
    y(0,2)=std::sin(angleRad);
    y(2,0)=-std::sin(angleRad);
    y(2,2)=std::cos(angleRad);
    return y;
}
inline Matrix4f Matrix4f::rotationZ(float angleRad){
    Matrix4f z;
    z(0,0)=std::cos(angleRad);
    z(0,1)=-std::sin(angleRad);
    z(1,0)=std::sin(angleRad);
    z(1,1)=std::cos(angleRad);
    return z;
}
inline Matrix4f Matrix4f::operator*(const Matrix4f& rm) const{
    Matrix4f ans;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            float sum=0;
            for(int k=0;k<4;k++){
                sum+=(*this)(i,k)*rm(k,j);
            }
            ans(i,j)=sum;
        }
    }
    return ans;
}
inline Vector3f Matrix4f::operator*(const Vector3f& v)const{
    Vector3f ans;
    float w=(*this)(3,0)*v(0)+(*this)(3,1)*v(1)+(*this)(3,2)*v(2)+(*this)(3,3);
    for(int i=0;i<3;i++){
        ans(i)=(*this)(i,0)*v(0)+(*this)(i,1)*v(1)+(*this)(i,2)*v(2)+(*this)(i,3);
    }
    if(w!=0){
        for(int i=0;i<3;i++){
            ans(i)/=w;
        }
    }
    return ans;
}


inline Vector3f::Vector3f(){
    x=0;y=0;z=0;
}
inline Vector3f::Vector3f(float a,float b,float c){
    x=a;y=b;z=c;
}
inline Vector3f& Vector3f::operator+=(const Vector3f& rv){
    (*this).x+=rv.x;
    (*this).y+=rv.y;
    (*this).z+=rv.z;
    return *this;
}
inline Vector3f& Vector3f::operator-=(const Vector3f& rv){
    (*this).x-=rv.x;
    (*this).y-=rv.y;
    (*this).z-=rv.z;
    return *this;
}
inline Vector3f operator+(Vector3f lv,const Vector3f& rv){
    return lv+=rv;
}
inline Vector3f operator-(Vector3f lv,const Vector3f& rv){
    return lv-=rv;
}
inline Vector3f Vector3f::operator*(float f)const{
    Vector3f ans=*this;
    ans.x*=f;
    ans.y*=f;
    ans.z*=f;
    return ans;
}
inline float Vector3f::dot(const Vector3f& rv)const{
    float ans=0;
    ans+=(*this).x*rv.x; 
    ans+=(*this).y*rv.y;
    ans+=(*this).z*rv.z;
    return ans;
}
inline Vector3f Vector3f::cross(const Vector3f& rv)const{
    Vector3f ans;
    ans.x=(*this).y*rv.z-(*this).z*rv.y;
    ans.y=(*this).z*rv.x-(*this).x*rv.z;
    ans.z=(*this).x*rv.y-(*this).y*rv.x;
    return ans;
}
inline float Vector3f::length()const{
    return std::sqrt(x*x+y*y+z*z);
}
inline Vector3f Vector3f::normalized()const{
    Vector3f ans;
    float L=(*this).length();
    if(L!=0){
        ans.x=x/L;
        ans.y=y/L;
        ans.z=z/L;
    }
    return ans;
}
inline const float& Vector3f::operator()(int i)const{
    if(i==0){
        return x;
    }else if(i==1){
        return y;
    }else if(i==2){
        return z;
    }else{
        std::cout<<"out of bounds access"<<std::endl;
        return x;
    }
}
inline float& Vector3f::operator()(int i){
    if(i==0){
        return x;
    }else if(i==1){
        return y;
    }else if(i==2){
        return z;
    }else{
        std::cout<<"out of bounds access"<<std::endl;
        return x;
    }
}
inline void Vector3f::show()const{
    std::cout<<x<<' '<<y<<' '<<z<<' '<<std::endl;
}


#endif