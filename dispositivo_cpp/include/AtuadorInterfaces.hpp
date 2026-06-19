#ifndef ATUADOR_INTERFACES_HPP
#define ATUADOR_INTERFACES_HPP

class IVaretasState {
    public:
        virtual bool isLigado() const = 0;
        virtual ~IVaretasState() {}
};

#endif // ATUADOR_INTERFACES_HPP
