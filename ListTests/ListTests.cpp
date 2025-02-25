#include <list>
#include <vector>
#include "CppUnitTest.h"
#include "my_list.h"


namespace ListTests { class SimpleElem; }
namespace ListTests { class NoCopyElem; }

//; ____________________________ ВАЖНО!!! ____________________________;
//  Для того, чтобы выполнить тестирование одного из указанных контейнеров (std::list или MyList),
//  должна быть раскомментирована одна из следующих строк:
//template<typename T> using ContainerTemplate = std::list<T>;
template<typename T> using ContainerTemplate = MyList<T>;

//; ____________________________ ТОЖЕ ВАЖНО!!! ____________________________;
// Для проверки бонусного задания необходимо заменить SimpleElem на NoCopyElem
using Elem = ListTests::SimpleElem;
//using Elem = ListTests::NoCopyElem;


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ListTests
{
	class SimpleElem {
		uint32_t watch;
		int data;
		static size_t elem_counter;
		void check() const { Assert::IsTrue(watch == 0xDEADBEEF, L"Повреждение памяти!! (Обращение к неинициализированному экземпляру класса SimpleElem)"); }
	public:
		SimpleElem(const SimpleElem& e) : data(e.data), watch(e.watch) { e.check(); ++elem_counter; }
		SimpleElem& operator=(const SimpleElem& e) { e.check();  data = e.data; watch = e.watch; }
		explicit SimpleElem(int value) : data(value), watch(0xDEADBEEF) { ++elem_counter; }
		SimpleElem(SimpleElem&& e) noexcept { e.check(); data = e.data; watch = e.watch; ++elem_counter; }
		SimpleElem& operator=(SimpleElem&& e) noexcept { check(); e.check(); data = e.data; watch = e.watch; }
		bool operator==(const SimpleElem& e) const { check(); e.check(); return data == e.data; }
		~SimpleElem() { check(); watch = 0; --elem_counter; }
		static size_t count() { return elem_counter; }
	};
	size_t SimpleElem::elem_counter = 0;

	class NoCopyElem {
		uint32_t watch;
		int data;
		static size_t elem_counter;
		void check() const { Assert::IsTrue(watch == 0xDEADDEAD, L"Повреждение памяти!! (Обращение к неинициализированному экземпляру класса NoCopyElem)"); }
	public:
		NoCopyElem(const NoCopyElem& e) = delete;
		NoCopyElem& operator=(const NoCopyElem& e) = delete;
		explicit NoCopyElem(int value) : data(value), watch(0xDEADDEAD) { ++elem_counter; }
		NoCopyElem(NoCopyElem&& e) noexcept { e.check(); data = e.data; watch = e.watch; ++elem_counter; }
		NoCopyElem& operator=(NoCopyElem&& e) noexcept { check(); e.check(); data = e.data; watch = e.watch; }
		bool operator==(const NoCopyElem& e) const { check(); e.check(); return data == e.data; }
		~NoCopyElem() { check(); watch = 0; --elem_counter; }
		static size_t count() { return elem_counter; }
	};
	size_t NoCopyElem::elem_counter = 0;


	TEST_CLASS(ComplexTypeTests)
	{

	public:

		TEST_METHOD(IteratorTests)
		{
			ContainerTemplate<int> list{ 3, 4, 8, 5, 2, 13, 20 };
			list.push_back(10);
			Assert::IsTrue(list.end() == ++--list.end(), L"++--list.end() должен возвращать list.end()");
			Assert::AreEqual(10, *(--list.end()), L"--list.end() должен возвращать последний элемент");
		}

		TEST_METHOD(StringTests)
		{
			ContainerTemplate<std::string> list{ "123", "AAAAAAAA", "abc", "cde" };
			std::vector<std::string> expected1{ "123", "AAAAAAAA", "abc", "cde" };

			Assert::AreEqual(std::size_t(4), list.size(), L"Неправильно считается количество строковых элементов");
			Assert::IsTrue(std::equal(list.begin(), list.end(), expected1.begin(), expected1.end()), L"Неправильный порядок строковых элементов");
			Assert::IsTrue(std::equal(list.rbegin(), list.rend(), expected1.rbegin(), expected1.rend()), L"Неправильный порядок строковых элементов");

			for (const auto& str : expected1)
				list.erase(std::find(list.begin(), list.end(), str));

			Assert::AreEqual(std::size_t(0), list.size(), L"Список не пуст после удаления всех элементов");
			for (const auto& elem : list)
				Assert::Fail(L"Что-то лежит в списке после удаления всех элементов");

			list.push_back("test-1");
			list.push_back("test-2");
			list.push_back("test-4");
			std::vector<std::string> expected2{ "test-1", "test-2", "test-4" };
			Assert::IsTrue(std::equal(list.begin(), list.end(), expected2.begin(), expected2.end()), L"Неправильный порядок строковых элементов");
			Assert::IsTrue(std::equal(list.rbegin(), list.rend(), expected2.rbegin(), expected2.rend()), L"Неправильный порядок строковых элементов");
		}

		TEST_METHOD(ElemTests)
		{
			size_t init_count = Elem::count();
			{
				ContainerTemplate<Elem> list;
				Assert::AreEqual(std::size_t(0), Elem::count() - init_count, L"Создан лишний экземпляр класса Elem");

				for (const auto& elem : list)
					Assert::Fail(L"Что-то лежит в пустом списке");

				list.push_back(Elem(40));
				list.push_back(Elem(75));
				list.push_front(Elem(50));
				list.push_back(Elem(23));
				list.push_back(Elem(87));
				Assert::IsTrue(Elem::count() - init_count == 5, L"Создан лишний экземпляр класса Elem");

				auto find_result = std::find(list.begin(), list.end(), Elem(75));
				Assert::IsTrue(find_result != list.end(), L"Не найден элемент со значением 75");
				Assert::IsTrue(*find_result == Elem(75), L"Неправильно найден элемент со значением 75");

				list.erase(find_result);
				Assert::IsTrue(Elem::count() - init_count == 4, L"Неправильно работает удаление элементов списка");
			}
			Assert::IsTrue(Elem::count() - init_count == 0, L"Утечка памяти!!");
		}

		TEST_METHOD(CopyElemTests)
		{
			size_t init_count = Elem::count();
			{
				ContainerTemplate<Elem> list;
				{
					ContainerTemplate<Elem> list1;
					ContainerTemplate<Elem> list2(std::move(list1));
					for (const auto& elem : list2)
						Assert::Fail(L"Что-то лежит в пустом списке после копирования");

					list2.push_back(Elem(40));
					list2.push_front(Elem(50));
					list2.push_back(Elem(23));
					list2.push_back(Elem(87));
					Assert::IsTrue(Elem::count() - init_count == 4, L"Неправильно работает удаление элементов списка");
					list = std::move(list2);
				}
				Assert::IsTrue(list.back() == Elem(87), L"Некправильно работает копирование списка");
				Assert::IsTrue(Elem::count() - init_count == 4, L"Неправильно работает удаление элементов списка");
			}
			Assert::IsTrue(Elem::count() - init_count == 0, L"Утечка памяти!!");
		}

		// TODO: добавить тесты для всех реализованных методов
		TEST_METHOD(ReverseTest)
		{
			ContainerTemplate<int> list{ 1,2,3,4,5 };
			list.reverse();
			ContainerTemplate<int> expected{ 5,4,3,2,1 };
			Assert::IsTrue(std::equal(list.begin(), list.end(), expected.begin(), expected.end()), L"Реверс не проходит");
			ContainerTemplate<int> list2;
			ContainerTemplate<int> expected2;
			list.reverse();
			Assert::IsTrue(std::equal(list2.begin(), list2.end(), expected2.begin(), expected2.end()), L"Реверс пустого не равен пустому");
		}

		TEST_METHOD(CountPosTest)
		{
			ContainerTemplate<int> half{ 1,2,3,4,-1 };
			ContainerTemplate<int> onluNeg{ -1,-2,-3 };
			ContainerTemplate<int> empty{ };
			ContainerTemplate<int> all{ 1,2,3,4};
			Assert::AreEqual(4, half.countPos());
			Assert::AreEqual(0, onluNeg.countPos());
			Assert::AreEqual(0, empty.countPos());
			Assert::AreEqual(4, all.countPos());
		}

		TEST_METHOD(CountMax)
		{
			ContainerTemplate<int> last{ 1,2,3,4,5 };
			ContainerTemplate<int> notlast{ 1,2,3,5,4 };
			ContainerTemplate<int> more{ 1,2,3,5,5 };
			ContainerTemplate<int> more2{ 5,2,3,5,5 };
			ContainerTemplate<int> first{ 5,4,3,2,1 };
			ContainerTemplate<int> empty{};
			{
				auto tuple = last.countMax();
				auto [x, y] = tuple;
				Assert::AreEqual(5, x);
				Assert::AreEqual(1, y);
			}
			{
				auto tuple = notlast.countMax();
				auto [x, y] = tuple;
				Assert::AreEqual(5, x);
				Assert::AreEqual(1, y);
			}
			{
				auto tuple = more.countMax();
				auto [x, y] = tuple;
				Assert::AreEqual(5, x);
				Assert::AreEqual(2, y);
			}
			{
				auto tuple = more2.countMax();
				auto [x, y] = tuple;
				Assert::AreEqual(5, x);
				Assert::AreEqual(3, y);
			}
			{
				auto tuple = first.countMax();
				auto [x, y] = tuple;
				Assert::AreEqual(5, x);
				Assert::AreEqual(1, y);
			}
			{
				auto tuple = empty.countMax();
				auto [x, y] = tuple;
				Assert::AreEqual(-1, x);
				Assert::AreEqual(-1, y);
			}
		}

		TEST_METHOD(SplitTest)
		{
			ContainerTemplate<int> normal{ -1,-2,-3,1,2,3 };
			ContainerTemplate<int> neg{ -1,-2,-3};
			ContainerTemplate<int> pos{1,2,3 };
			ContainerTemplate<int> empty{};
			{
				auto tuple = normal.split();
				ContainerTemplate<int> x = tuple.first;
				ContainerTemplate<int> y = tuple.second;
				ContainerTemplate<int> exp1 = { -1,-2,-3 };
				ContainerTemplate<int> exp2 = { 1,2,3 };
				Assert::IsTrue(std::equal(x.begin(), x.end(), exp1.begin(), exp1.end()));
				Assert::IsTrue(std::equal(y.begin(), y.end(), exp2.begin(), exp2.end()));
			}
			{
				auto tuple = neg.split();
				ContainerTemplate<int> x = tuple.first;
				ContainerTemplate<int> y = tuple.second;
				ContainerTemplate<int> exp1 = { -1,-2,-3 };
				ContainerTemplate<int> exp2 = {};

				Assert::IsTrue(std::equal(x.begin(), x.end(), exp1.begin(), exp1.end()));
				Assert::IsTrue(std::equal(y.begin(), y.end(), exp2.begin(), exp2.end()));
			}
			{
				auto tuple = pos.split();
				ContainerTemplate<int> x = tuple.first;
				ContainerTemplate<int> y = tuple.second;
				ContainerTemplate<int> exp1 = {};
				ContainerTemplate<int> exp2 = { 1,2,3 };
				Assert::IsTrue(std::equal(x.begin(), x.end(), exp1.begin(), exp1.end()));
				Assert::IsTrue(std::equal(y.begin(), y.end(), exp2.begin(), exp2.end()));
			}
			{
				auto tuple = empty.split();
				ContainerTemplate<int> x = tuple.first;
				ContainerTemplate<int> y = tuple.second;
				ContainerTemplate<int> exp1 = {};
				ContainerTemplate<int> exp2 = {};
				Assert::IsTrue(std::equal(x.begin(), x.end(), exp1.begin(), exp1.end()));
				Assert::IsTrue(std::equal(y.begin(), y.end(), exp2.begin(), exp2.end()));
			}
		}

		TEST_METHOD(PopFrontTest)
		{
			ContainerTemplate<int> empty;
			ContainerTemplate<int> emptyToo;
			empty.pop_front();
			Assert::IsTrue(std::equal(empty.begin(), empty.end(), emptyToo.begin(), emptyToo.end()));
			empty.push_back(1);
			empty.pop_front();
			Assert::IsTrue(std::equal(empty.begin(), empty.end(), emptyToo.begin(), emptyToo.end()));
			ContainerTemplate<int> full{ 1,2,3,4,5 };
			ContainerTemplate<int> exp{ 2,3,4,5 };
			full.pop_front();
			Assert::IsTrue(std::equal(full.begin(), full.end(), exp.begin(), exp.end()));
		}

		TEST_METHOD(PopBackTest)
		{
			ContainerTemplate<int> empty;
			ContainerTemplate<int> emptyToo;
			empty.pop_back();
			Assert::IsTrue(std::equal(empty.begin(), empty.end(), emptyToo.begin(), emptyToo.end()));
			empty.push_back(1);
			empty.pop_back();
			Assert::IsTrue(std::equal(empty.begin(), empty.end(), emptyToo.begin(), emptyToo.end()));
			ContainerTemplate<int> full{ 1,2,3,4,5 };
			ContainerTemplate<int> exp{ 1,2,3,4 };
			full.pop_back();
			Assert::IsTrue(std::equal(full.begin(), full.end(), exp.begin(), exp.end()));
		}
	};
}
