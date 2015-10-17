#ifndef __ISERVERBASE_H__
#define __ISERVERBASE_H__

class iServerBase
{
public:
    iServerBase();
    virtual ~iServerBase() = 0;

    virtual void Init() = 0;
    virtual void Fini() = 0;

    void Run();

private:

};

#endif
