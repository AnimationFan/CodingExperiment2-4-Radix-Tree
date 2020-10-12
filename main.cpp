#include"common.h"
#include"RadixTree.h"
#include"FileOption.h"



void testMatch() {
	int result = 0;
	int sum = 6;
	char* s[] = {"a","b","cd","f","abbb","bbbb"};
	char* t[] = { "a","bc","c","f","bbf","bbf"};
	int begin[] = { 3,3,3,2,3,3 };
	int length[] = {2,5,5,6,29,29};
	int predict[] = { 2,5,5,6,3,18 };
	for (int i = 0; i < sum; i++) {
		RadixTreeNode* node = new RadixTreeNode(s[i],begin[i],length[i],true);
		int matchLen = node->match(t[i],begin[i]);
		printf("predict:%d actual:%d \n",predict[i], matchLen);
	}

}
void testStrCut() {
	char* str = "abcd";
	for (int i = 0; i < 30; i++) {
		char* result = RadixTreeNode::cutStr(str,8,i);
		if (result != nullptr) {
			int len = strlen(result);
			printf("起始8bit, 长度位%dBit时,截切的结果为",i);
			int beginChar = 8 >> 3;
			int endChar = (8 + i) >> 3;
			int charLen = endChar - beginChar + 1;
			for (int j = 0; j < charLen; j++) 
				printf("%c", result[j]);
			printf("\n");
		}
		else
			printf("起始8bit, 长度位%dBit时，参数超出限制\n", i);
	}
}


void testAdjust() {
	int sum = 5;
	char* s[] = { "ab","b","cd","f","abbb", };
	char* t[] = { "a","bc","c","f","bbf", };
	
	int begin[] = { 0, 1, 2, 3,4 };
	int length[] = { 2,5,5, 5,28};

	int nodePre[] = {2,5,5,5,2, };
	for (int i = 0; i < sum; i++) {
		RadixTreeNode* node = new RadixTreeNode(s[i], begin[i], length[i], true);
		node->adjust(t[i], begin[i]);
		printf("predict:%d actual:%d\n", nodePre[i], node->length);
		
	}
}

void testPut() {
	int sum = 7;
	char* str[] = {"aaaa","a","b","bc","abc","bcd","acd"};
	RadixTree* tree = new RadixTree();
	for (int i = 0; i < sum; i++) {
		tree->put(str[i]);
	}
	char  buffer[BUFFER_SIZE];
	memset(buffer,0x00,BUFFER_SIZE);
	tree->getRoot()->visist(buffer,0);
	sum = 0;
}

void testGet() {
	int sum = 7;
	char* str[] = { "aaaa","a","b","bc","abc","bcd","acd" };
	RadixTree* tree = new RadixTree();
	for (int i = 0; i < sum; i++) {
		tree->put(str[i]);
	}
	int tar = 14;
	char* tstr[] = { "x","y","z","bf","bd","aac","ac","aaaa","a","b","bc","abc","bcd","acd" };
	for (int i = 0; i < 14; i++) {
		if (tree->get(tstr[i]))
			printf("get %s success\n", tstr[i]);
	}
}



void subTest() {
	RadixTreeNode* a = new RadixTreeNode();
	RadixTreeNode* b = new RadixTreeNode();
	b->left = nullptr;
	a->right = nullptr;
	a->left = b->left;
	a->right = b->right;
}


char* sourceFileLoc = { "dict.txt" };
char* targetFileLoc = { "string.txt" };
char* resultFileLoc = { "bupt_18_4.txt" };



int main() {
	time_t  start, finish;
	start = clock();


	RadixTree* tree = new RadixTree();
	FileReader* sourceFile = new FileReader();
	bool openRe = sourceFile->openFile(sourceFileLoc, "r");
	if (openRe == false) {
		printf("源文件打开错误\n");
		return 0;
	}

	//写入MTrie
	char* data = new char[BUFFER_SIZE];
	memset(data, 0x00, BUFFER_SIZE);
	while (sourceFile->getline(data))
	{
		tree->put(data);
		memset(data, 0x00, BUFFER_SIZE);
	}
	sourceFile->closeFile();
	delete sourceFile;


	//打开待匹配字符串文件
	FileReader* targetFile = new FileReader();
	openRe = targetFile->openFile(targetFileLoc, "r");
	if (openRe == false) {
		printf("目标文件打开错误\n");
		return 0;
	}
	//打开匹配结果文件准备写入
	FileWriter* fileWriter = new FileWriter();
	openRe = fileWriter->openFile(resultFileLoc, "w");
	if (openRe == false) {
		printf("打开结果文件错误\n");
		return 0;
	}
	memset(data, 0x00, BUFFER_SIZE);
	int i = 0;
	while (targetFile->getline(data))
	{
		int length = strlen(data);
		if (length == 0) continue;
		int getRe = tree->get(data);
		if (getRe == 1) {

			fileWriter->putline(data);
			i++;
		}
		memset(data, 0x00, BUFFER_SIZE);
	}
	finish = clock();
	delete targetFile;
	delete fileWriter;
	delete[] data;
	delete tree;


	printf("string_march:%d  runtime:%d\n", i, (finish - start));


	system("pause");
	_sleep(3000);
	return 0;
}

