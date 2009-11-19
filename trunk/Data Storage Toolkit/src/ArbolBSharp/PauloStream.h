/*
 * PauloStream.h
 *
 *  Created on: 19/11/2009
 *      Author: paulo
 */

#ifndef PAULOSTREAM_H_
#define PAULOSTREAM_H_
#include <fstream>
class PauloStream : public std::fstream{
public:
	std::ostream& seekp ( std::streampos pos ){
		if(pos==0)
			cout<<endl;
		return fstream::seekp(pos);

	}
	std::ostream& seekp ( std::streamoff off, std::ios_base::seekdir dir ){
		if(off==0 and dir==std::ios_base::beg )
			cout<<endl;
		return fstream::seekp(off,dir);
	};
	virtual ~PauloStream(){};
};

#endif /* PAULOSTREAM_H_ */
