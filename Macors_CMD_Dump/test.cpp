#include <iostream>  
#include "DumpFile.h"


void test()  
{  
	std::string s = "abcd";  

	try{  
		s[100] = 'b';  
	}  
	catch(std::exception& e)  
	{  
		std::cout << "with exception:[" << e.what() << "]" << std::endl;  
	}  
	catch(...)  
	{  
		std::cout << "with unknown exception" << std::endl;  
	}  
}  

void main()  
{  
    DeclareDumpFile();	//ֻ�����������ʼ��ʱ����DeclareDumpFile(); ���ɲ���dump��
	test();  

	system("pause");  
}  