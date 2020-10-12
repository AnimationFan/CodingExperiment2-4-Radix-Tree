#pragma once

class RadixTreeNode {
public:
	/*��ǰ����Ӧ���ַ���*/
	char* str;
	/*��ǰ�ַ�����Ӧ����ʼλ*/
	int beginBit;
	/*����б���Ķ�Ӧ������λ�ĳ���*/
	int length;
	/*��ʾ��ǰ����Ƿ�Ϊ���ַ����Ľ�β*/
	bool itemEnd;
	/*��������������*/
	RadixTreeNode* left, *right;
	
	/*���캯��������յĽ��*/
	RadixTreeNode();
	/*���캯��������ֱ���趨������Ҫ�Ĳ���*/
	RadixTreeNode(char* str, int beginBit, int length, bool itemEnd);
	/*ƥ�亯������������ַ�������ʼλ��ʼ�뱾��洢���ַ�������ʼλ�Ƚ�
		���������ƥ��ĳ���
	*/
	int match(char* str, int beginBit);
	/*�����������������Ƿ��ѽ�㣬��
		����������ַ�����������Ϊ���ַ�����ȫƥ��ĺͲ�ƥ����������֣�
		����Ľ�㽫�����ƥ��Ľ���
	*/
	bool adjust(char* str, int beginBit);

	/*��ȡ�ַ�������*/
	static char* cutStr(char* str, int beginBit, int lenBit);
	/*�����Ե�ǰ���Ϊ����RadxiTree*/
	void visist(char* buffer,  int curChar);
};

class RadixTree
{
private:
	/*���ڵ�*/
	RadixTreeNode* root;
public:
	/*���캯��*/
	RadixTree();
	RadixTreeNode* getRoot();
	/*���ַ���*/
	bool put(char* str);
	/*ȡ�ַ���*/
	bool get(char* str);
};

