#ifndef COMPONENTEMEMORIA_H_
#define COMPONENTEMEMORIA_H_

class ComponenteMemoria{
public:
	bool estaEscrito()const{
		return estado&escrito;
	};
	bool estaSucio()const{
		return estado&sucio;
	};
	void setSucio(bool valor){
		if(valor) estado =flags(estado|sucio);
			else estado=flags(estado&limpio);
	};
	void setEscrito(bool valor){
		if(valor) estado =flags(estado|escrito);
			else estado=flags(estado&noescrito);
	};
protected:
	enum flags {limpio=0x2,sucio=0x01,escrito=0x02,noescrito=0x1} estado;
};

#endif /* COMPONENTEMEMORIA_H_ */
