#ifndef _PLATAFORMA_EMBEDDED_HPP
#define _PLATAFORMA_EMBEDDED_HPP


class PlataformaEmbedded{
  public:
    virtual void mainLoop(void);
    friend void main (void);
};


#endif