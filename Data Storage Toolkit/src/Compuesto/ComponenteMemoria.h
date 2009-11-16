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
			else estado=flags(estado|(!sucio));
	};
	void setEscrito(bool valor){
		if(valor) estado =flags(estado|escrito);
			else estado=flags(estado|(!escrito));
	};
protected:
	enum flags {sucio=0x01,escrito=0x02} estado;
};

#endif /* COMPONENTEMEMORIA_H_ */
