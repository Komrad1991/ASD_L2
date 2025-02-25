#include <iostream>
#include <list>
#include "my_list.h"
#include <vector>
#include <random>
#include <chrono>
#include <time.h>

using namespace std::chrono;

void push_back_time()
{
	MyList<std::vector<std::vector<int>>>* my_list = new MyList<std::vector<std::vector<int>>>();
	std::list<std::vector<std::vector<int>>> list;

	// Тут надо сравнить работу своего контейнера со стандартным

	std::vector<std::vector<int>> big_sh;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<std::mt19937::result_type> dist(10, 1000);
	int a = 800;
	int b = 120;
	std::cout << a << " * " << b << std::endl;
	for (int i = 0; i < a; i++)
	{
		big_sh.push_back(std::vector<int>());
		for (int j = 0; j < b; j++)
			big_sh[i].push_back(dist(gen));
	}
	std::cout << "gen compl" << std::endl;
	std::vector<std::vector<int>> big_sh2(big_sh);
	long long duration = 0;
	for (int i = 0; i < 50; i++)
	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		for (int j = 0; j < 100; j++)
		{

			my_list->push_back(big_sh);
		}
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration += std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	}
	duration /= 50;
	std::cout << "My_list push_back: " << duration << std::endl;
	duration = 0;
	for (int i = 0; i < 50; i++)
	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		for (int j = 0; j < 100; j++)
		{
			list.push_back(big_sh);
		}
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration += std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	}
	duration /= 50;
	std::cout << "List push_back: " << duration << std::endl;
	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		my_list->clear();
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration += std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		std::cout << "My list clear: " << duration << std::endl;
	}

	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		list.clear();
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration += std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		std::cout << "list clear: " << duration << std::endl;
	}
}

void push_front_time()
{
	MyList<std::vector<std::vector<int>>>* myOrig = new MyList<std::vector<std::vector<int>>>();
	std::list<std::vector<std::vector<int>>> orig;

	// Тут надо сравнить работу своего контейнера со стандартным

	std::vector<std::vector<int>> big_sh;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<std::mt19937::result_type> dist(10, 1000);
	int a = 1000;
	int b = 120;
	std::cout << a << " * " << b << std::endl;
	for (int i = 0; i < a; i++)
	{
		big_sh.push_back(std::vector<int>());
		for (int j = 0; j < b; j++)
			big_sh[i].push_back(dist(gen));
	}
	std::cout << "gen compl" << std::endl;
	std::vector<std::vector<int>> big_sh2(big_sh);
	long long duration = 0;
	for (int i = 0; i < 50; i++)
	{
		auto my_list = myOrig;
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		for (int j = 0; j < 100; j++)
		{
			my_list->push_front(big_sh);
		}
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration += std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	}
	duration /= 50;
	std::cout << "My_list push_front: " << duration << std::endl;
	duration = 0;
	for (int i = 0; i < 50; i++)
	{
		auto list = orig;
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		for (int j = 0; j < 100; j++)
		{
			list.push_front(big_sh);
		}
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration += std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	}
	duration /= 50;
	std::cout << "List push_front: " << duration << std::endl;
	duration = 0;
	for (int i = 0; i < 100; i++)
	{
		auto copy = myOrig;
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		for (int j = 0; j < 100; j++)
		{
			copy->insert(copy->begin()++, big_sh);
		}
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration += std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	}
	duration /= 100;
	std::cout << "MyList insert 100: " << duration << std::endl;
	duration = 0;
	for (int i = 0; i < 100; i++)
	{
		auto copy = orig;
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		for (int j = 0; j < 100; j++)
		{
			copy.insert(copy.begin()++, big_sh);
		}
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration += std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	}
	duration /= 100;
	std::cout << "List insert 100: " << duration << std::endl;
}

int main(int argc, char** argv)
{
	push_back_time();
	std::cout << "-----" << std::endl;
	push_front_time();
	// Ещё можно проверять работоспособность методов, но для
	// каждого метода надо описать тест в проекте с тестами
}


