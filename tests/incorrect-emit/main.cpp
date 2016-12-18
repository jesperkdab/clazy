#include <QtCore/QObject>

class MyObject;

MyObject* somefunc()
{
    return nullptr;
}

static MyObject * s_obj;

class MyObject : public QObject
{
public:
    void pub();
    MyObject* memberFunc() const;
    MyObject *another;

private:
    void priv();

public slots:
    void pubSlot();

signals:
    void sig();

private Q_SLOTS:
    void privSlot();

protected:
    void prot();
};

void MyObject::pub()
{
    emit  prot(); // Warning: emit on non slot.
    sig(); // Warning: missing emit
    prot(); // OK
    pub(); // OK
    priv(); // OK
    privSlot(); // OK
    Q_EMIT privSlot(); // Warning
    Q_EMIT somefunc()->sig(); // OK
    somefunc()->sig(); // Warning
    Q_EMIT memberFunc()->sig(); // OK
    memberFunc()->sig(); // Warning
    emit another->sig(); // OK
    emit s_obj->sig(); // OK
}
