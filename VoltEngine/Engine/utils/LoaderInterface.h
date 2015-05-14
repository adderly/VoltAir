#ifndef LOADERINTERFACE
#define LOADERINTERFACE

enum LoaderCodes:uint{
    no_error,
    stateless_error,
    file_error,
    custom_error
};

/**
* Interface for generalizing "rules" when loading any loadable object.
*/
class LoaderInterface{
    virtual bool load() = 0;
    virtual uint errorCode() = 0;
};


#endif // LOADERINTERFACE

