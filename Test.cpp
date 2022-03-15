#include <iostream>
#include <vector>
#include <assert.h>
#include <future>
#include <execution>
#include <fstream>
#include <string>
#include <tuple>

using namespace std;

class FullName
{
public:
    FullName() = default;
    FullName(string _firstName, string _patronymic, string _lastName) : FirstName(_firstName), Patronymic(_patronymic), LastName(_lastName) {}
    FullName(string _firstName, string _lastName) : FirstName(_firstName), LastName(_lastName) {}
    string getFName()
    {
        if (Patronymic.empty())
        {
            return FirstName + " " + LastName + " ";
        }
        else
        {
            return FirstName + " " + Patronymic + " " + LastName + " ";
        }
    }

    friend ostream& operator <<(ostream& out, const FullName& name)
    {
        if (name.Patronymic.empty())
        {
            return out << name.FirstName << " " << name.LastName << " ";
        }
        else
        {
            return out << name.FirstName << " " << name.Patronymic << " " << name.LastName << " ";
        }
    }
    friend bool operator == (FullName first, FullName second)
    {
        if (first.FirstName == second.FirstName && first.LastName == second.LastName && first.Patronymic == second.Patronymic)
        {
            return true;
        }
        return false;
    }

protected:
    string FirstName;
    string Patronymic;
    string LastName;
};

class Student
{
public:
    Student(const FullName& _Name, vector<int> _Marks) : Name(_Name), Marks(_Marks), Average_mark(Average_markCalcul(_Marks)) {}
    double Average_markCalcul(vector<int> _Marks)
    {
        double av = 0;
        for (size_t i = 0; i < _Marks.size(); i++)
        {
            av += _Marks[i];
        }
        av /= _Marks.size();
        return av;
    }
    FullName getFullName()
    {
        return Name;
    };
    double getAverage_mark()
    {
        return Average_mark;
    };

    friend ostream& operator <<(ostream& out, const Student stud)
    {
        return out << stud.Name << " Среднее значение = " << stud.Average_mark;
    }

private:
    const FullName Name;
    vector<int> Marks;
    double Average_mark;
};

class IRepository
{
    virtual void Open() = 0;
    virtual void Save() = 0;
};

class IMethods
{
    virtual double GetAverageScore(const FullName& name) = 0;
    virtual string GetAllInfo(const FullName& name) = 0;
    virtual string GetAllInfo() = 0;
};

class StudentsGroup : public IRepository, public IMethods
{
public:
    StudentsGroup(vector<Student> _StudGroup) : StudGroup(_StudGroup) {}
    string GetAllInfo() override
    {
        string AllInfo;
        for (size_t i = 0; i < StudGroup.size(); i++)
        {
            AllInfo += StudGroup[i].getFullName().getFName() + " Среднее значение = " + to_string(StudGroup[i].getAverage_mark()) + "\n";
        }
        return AllInfo;
    }
    double GetAverageScore(const FullName& name) override
    {

        for (size_t i = 0; i < StudGroup.size(); i++)
        {
            if (StudGroup[i].getFullName() == name)
            {
                return StudGroup[i].getAverage_mark();
            }
        }
        return 0;
    }
    string GetAllInfo(const FullName& name) override
    {
        string AllInfo = "Подходящее значение не найдено!";
        for (size_t i = 0; i < StudGroup.size(); i++)
        {
            if (StudGroup[i].getFullName() == name)
            {
                AllInfo = StudGroup[i].getFullName().getFName() + " Среднее значение = " + to_string(StudGroup[i].getAverage_mark());
            }
        }
        return AllInfo;
    }
    void Save() override
    {
        ofstream out("StudenstGroup.bin", ios::binary);
        out.write(GetAllInfo().c_str(), GetAllInfo().size());
    }
    void Open() override
    {
        string result;
        ifstream in("StudenstGroup.bin", ios::binary);
        result.resize(GetAllInfo().size());
        in.read(&result[0], result.size());
        result.resize(in.gcount());
        cout << result;
    }

private:
    vector<Student> StudGroup;
};

template<typename T>
T min_el(const vector<T>& v)
{
    return *min_element(v.begin(), v.end());
}

template<typename T>
T max_el(const vector<T>& v)
{
    return *max_element(v.begin(), v.end());
}

template<typename T, typename U>
void AssertEq(const T& t, const U& u)
{
    if (t != u)
    {
        ostringstream os;
        os << "Assertion failed" << t << "!= " << u;
        throw runtime_error(os.str());
    }
}

template<typename TestFunc>
void RunTest(TestFunc func, const string& test_name)
{
    try {
        func();
        cerr << test_name << "OK" << endl;
    }
    catch (runtime_error& ex) {
        cerr << test_name << "Fail: " << ex.what() << endl;
    }
}

FullName StudentName(Student stud)
{
    return stud.getFullName();
}

void Test_min_el(StudentFullName, StudentName)
{
    vector<int> Scores{ 25,89,21,2,67,45,10,24,1 };
    Student First(FullName("Руслан", "Казалаков", "Игоревич"), Scores);
    FullName Ruslan("Руслан", "Казалаков", "Игоревич");
    AssertEq(Ruslan, StudentName(First));
    Student Second(FullName("Ольга", "Маргарита", "Абрамс"), Scores);
    FullName Olga("Ольга", "Маргарита", "Стивенс");
    AssertEq(Olga, StudentName(Second));
}

int main()
{
    setlocale(LC_ALL, "Russian");
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}