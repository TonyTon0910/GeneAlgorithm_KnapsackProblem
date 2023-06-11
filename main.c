#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define ITEMSIZE 2100
#define GENESIZE 2100

struct item {
	int size;
	int value;
};
struct gene {
	int select[ITEMSIZE];
	int size;
	int value;
};
struct geneList {
	gene *list;
	int size;
};
using namespace std;
vector<item> itemTable;

const std::vector<std::string> split(const std::string& str, const std::string& pattern) {
	std::vector<std::string> result;
	std::string::size_type begin, end;

	end = str.find(pattern);
	begin = 0;

	while (end != std::string::npos) {
		if (end - begin != 0) {
			result.push_back(str.substr(begin, end - begin));
		}
		begin = end + pattern.size();
		end = str.find(pattern, begin);
	}

	if (begin != str.length()) {
		result.push_back(str.substr(begin));
	}
	return result;
}
void readFile(string fileName) {
	string line;
	ifstream myFile;
	myFile.open(fileName);
	int i = -1;
	while (getline(myFile, line)) {
		i++;
		if (i == 0)
			continue;
		string pattern = ",";
		vector<string> ret = split(line, pattern);
		item t;
		t.size = stoi(ret[1]);
		t.value = stoi(ret[2]);
		itemTable.push_back(t);
	}
	myFile.close();
}
void getSizeAndCost(gene* g) {
	g->size = 0;
	g->value = 0;
	for (int i = 0; i < itemTable.size(); i++) {
		g->size += itemTable[i].size * g->select[i];
		g->value += itemTable[i].value * g->select[i];
	}
}
geneList getInitSolution(int N) {
	geneList V;
	V.list = new gene[GENESIZE];
	for (int i = 0; i < N; i++) {
		gene g;
		g.size = 0;
		g.value = 0;
		for (int i = 0; i < itemTable.size(); i++) {
			//g.select[i] = rand() % 2;
			g.select[i] = 0;
		}
		getSizeAndCost(&g);
		V.list[i] = g;
		if (g.size > 500) {
			i--;
		}
	}
	V.size = N;
	return V;
}

void geneExchange(geneList* V) {
	int lower = 1;
	int upper = itemTable.size() / 10;
	int range = rand() % (upper - lower + 1) + lower;
	int e1 = rand() % V->size;
	int e2 = rand() % V->size;
	
	for (int i = 0; i < itemTable.size(); i++) {
		V->list[V->size].select[i] = V->list[e1].select[i];
	}
	for (int i = 0; i < range; i++) {
		int r = rand() % itemTable.size();
		V->list[V->size].select[r] = V->list[e2].select[r];
	}
	getSizeAndCost(&V->list[V->size]);
	//V->size++;
}
void geneSelfChange(geneList* V) {
	int lower = 1;
	int upper = itemTable.size() / 5;
	int range = rand() % (upper - lower + 1) + lower;
	int e = rand() % V->size;

	for (int i = 0; i < itemTable.size(); i++) {
		V->list[V->size].select[i] = V->list[e].select[i];
	}
	for (int i = 0; i < range; i++) {
		int r = rand() % itemTable.size();
		V->list[V->size].select[r] = rand() % 2;
	}
	getSizeAndCost(&V->list[V->size]);
	//V->size++;
}
void generateNewGene(geneList* V,int G) {
	for (int i = 0; i < G; i++) {

		if (rand() % 2)
			geneExchange(V);
		else
			geneSelfChange(V);
		if (V->list[V->size].size > 500) {
			i--;
		}
		else
			V->size++;
	}
}
void swap(gene *a, gene* b) {
	gene temp = *a;
	*a = *b;
	*b = temp;
}

void quickSort(geneList* array, int startPos, int endPos) {
	int length = endPos - startPos;
	if (length < 1)
		return;
	//如果陣列大小小於70就做bubble sort
	/*if (length < 70) {
		for (int i = 0; i <= endPos - startPos; i++)
			for (int j = startPos; j <= endPos - i; j++)
				if (array->list[i].value > array->list[j].value)
					swap(&array->list[i], &array->list[j]);
		return;
	}*/
	//pivit設定為最後一個
	int pivit = array->list[endPos].value;
	int pivitPos = startPos;
	//比pivit大的放左邊 剩下放右邊
	for (int i = startPos; i <= endPos; i++)
		if (array->list[i].value >= pivit) {
			swap(&array->list[i], &array->list[pivitPos]);
			pivitPos++;
		}
	//pivitPos的位置實際上在pivit的右邊一格
	quickSort(array, startPos, pivitPos - 2);
	quickSort(array, pivitPos, endPos);
}
void Rank(geneList* V) {
	quickSort(V, 0, V->size - 1);
}
gene geneAlgo(int N,int G) {

	int times = 3000;
	geneList V = getInitSolution(N);

	for (int i = 0; i < times; i++) {
		//generate number of G gene
		generateNewGene(&V,G); 
		Rank(&V);
		//reserve first N gene in V;
		V.size = N; 
		if (i % 100 == 0)
			cout << i << endl;
	}
	return V.list[0];
}
int main() {
	readFile("backpage.csv");
	//init is 100,new gene is 100
	gene best = geneAlgo(1000,500);
	cout <<best.value<< endl;
}