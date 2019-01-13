#ifndef QPX_STABLELIST_H
#define QPX_STABLELIST_H

#include <QtCore/QVector>
#include <QtCore/QSet>

namespace QPx
{

template<typename T> class StableList
{
public:
    class iterator
    {
    public:
        bool operator==(const iterator &o) const { return i == o.i; }
        bool operator!=(const iterator &o) const { return i != o.i; }

        iterator &operator++(){ i = s->nextValid(i + 1); return *this; }
        iterator operator++(int){ auto r = *this; i = s->nextValid(i + 1); return r; }

        T &operator*() const { return (*s)[i]; }
        T *operator->() const { return &(*s[i]); }

    private:
        friend class StableList<T>;

        iterator(StableList<T> *s, int i) : s(s), i(i) { }

        StableList<T> *s;
        int i;
    };

    class const_iterator
    {
    public:
        bool operator==(const const_iterator &o) const { return i == o.i; }
        bool operator!=(const const_iterator &o) const { return i != o.i; }

        const_iterator &operator++(){ i = s->nextValid(i + 1); return *this; }
        const_iterator operator++(int){ auto r = *this; i = s->nextValid(i + 1); return r; }

        const T &operator*() const { return (*s)[i]; }
        const T *operator->() const { return &(*s[i]); }

    private:
        friend class StableList<T>;

        const_iterator(const StableList<T> *s, int i) : s(s), i(i) { }

        const StableList<T> *s;
        int i;
    };

    StableList() : last(0) { }

    void add(const T &t);
    void remove(int index);

    T &operator[](int index){ return vv[index]; }
    const T &operator[](int index) const { return vv[index]; }

    iterator begin(){ return iterator(this, nextValid(0)); }
    iterator end(){ return iterator(this, last); }

    const_iterator begin() const { return const_iterator(this, nextValid(0)); }
    const_iterator end() const { return const_iterator(this, last); }

private:
    friend class iterator;

    int nextValid(int index) const;

    QVector<T> vv;
    QSet<int> fs;
    int last;
};

template<typename T> void StableList<T>::add(const T &t)
{
    if(fs.empty())
    {
        vv.push_back(t);
        last = vv.count();
    }
    else
    {
        auto index = *fs.begin();
        fs.erase(fs.begin());

        vv[index] = t;

        if(index + 1 > last) last = index + 1;
    }
}

template<typename T> void StableList<T>::remove(int index)
{
    vv[index] = T();
    fs.insert(index);

    if(last == index + 1)
    {
        while(true)
        {
            if(last == 0 || fs.find(last - 1) == fs.end()) break;
            --last;
        }
    }
}

template<typename T> int StableList<T>::nextValid(int index) const
{
    while(index < last && fs.find(index) != fs.end()) ++index;
    return index;
}

}

#endif // QPX_STABLELIST_H
