#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

class Person
{
    public :
        string name;
        int age;

        Person():name(""),age(0){}

        virtual void getdata()
        {
            cin>>name>>age;
        }

        virtual void putdata()
        {
            cout<<name<<" "<<age;
        }
};
class Professor : public Person
{
    public:
    int publication,cur_id;
    static int count_professor;
    Professor():Person(),publication(0)
    {
        count_professor++;
        cur_id = count_professor;
    }

    virtual void getdata()
    {
        cin>>name>>age>>publication;
    }

    virtual void putdata()
    {
        cout<<name<<" "<<age<<" "<<publication<<" "<<cur_id<<endl;
    }

};

int Professor::count_professor = 0;


class Student : public Person
{
    public:
    int mark[6],cur_id;
    static int count_student;
    Student():Person()
    {
        count_student++;
        cur_id = count_student;
    }

    virtual void getdata()
    {
        cin>>name>>age>>mark[0]>>mark[1]>>mark[2]>>mark[3]>>mark[4]>>mark[5];
    }

    virtual void putdata()
    {
        int sum = 0;
        for(int i = 0;i<6;i++)
        {
            sum = sum + mark[i];
        }
        cout<<name<<" "<<age<<" "<<sum<<" "<<cur_id<<endl;
    }

};

int Student::count_student = 0;

int main(){

    int n, val;
    cin>>n; //The number of objects that is going to be created.
    Person *per[n];

    for(int i = 0;i < n;i++){

        cin>>val;
        if(val == 1){
            // If val is 1 current object is of type Professor
            per[i] = new Professor;

        }
        else per[i] = new Student; // Else the current object is of type Student

        per[i]->getdata(); // Get the data from the user.

    }

    for(int i=0;i<n;i++)
        per[i]->putdata(); // Print the required output for each object.

    return 0;

}
