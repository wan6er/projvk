#include "test_header.h"
#include "utils/base_obj.h"
#include "utils/cloned_obj.h"
#include "utils/vertex_data.h"

static int __count = 0;

class Object
{
public:
    Object() 
    {
        __count++;
    }

    ~Object() 
    {
        __count--;
    }

    int a = 0;
};

class TestObj : public utils::BaseObj<Object>
{
public:
    TestObj() = default;
    TestObj(const Object& obj) :
        utils::BaseObj<Object>(obj)
    {

    }
    ~TestObj() {
        if (isolated() && !is_cloned()) {
            static int __rel_count = 0;
            __rel_count++;
            CHECK(__rel_count == 1);
        }
    }
};

TEST_FUNC_BEGIN("object copy")

    TestObj obj;
    {
        CHECK_EQ(obj.count(), __count);
        {
            TestObj obj1 = obj;
            // CHECK(obj1.is_cloned());
            CHECK((obj.count() == 2 && obj.count() == __count));
        }
            CHECK((obj.count() == 1 && obj.count() == __count));
        TestObj obj2(obj);
        // CHECK(obj2.is_cloned());
        CHECK_EQ(obj.count(), __count);
    }
    CHECK((obj.count() == 1 && obj.count() == __count));

TEST_FUNC_END


#include "utils/def_utils.h"
TEST_FUNC_BEGIN("def utils")
    CHECK(CVK_DEF_COUNT_ARGS() == 1);
    CHECK(CVK_DEF_COUNT_ARGS(a) == 1);
    CHECK(CVK_DEF_COUNT_ARGS(1, a) == 2);
    CHECK(CVK_DEF_COUNT_ARGS(a, b, 3) == 3);
    CHECK(CVK_DEF_COUNT_ARGS(asdo1, 218, aw8h1) == 3);
TEST_FUNC_END

TEST_FUNC_BEGIN("vertex data")
    struct test { int a; char b; int c; float d; };
    utils::VectexData<test> data(VERTEX_DATA_REG_MEMBERS(test, a, b, c, d));
    int a = 10;
    data.append("a", &a);
    a = 11;
    data.append("a", &a);
    a = 12;
    data.append("a", &a);
    data.set(1, "c", &a);
    CHECK(data[0].a == 10);
    CHECK(data[0].b == 0);
    CHECK(data[1].a == 11);
    CHECK(data[1].b == 0);
    CHECK(data[1].c == 12);
TEST_FUNC_END


#include "utils/tuple.h"
#include "utils/tuple_union.h"
TEST_FUNC_BEGIN("tuple")

    utils::Tuple vals2(3, 1.0f, 'a', 10);
    CHECK(utils::get<0>(vals2) == 3);
    CHECK(utils::get<1>(vals2) == 1.0f);
    CHECK(utils::get<2>(vals2) == 'a');

    CHECK(std::is_same_v<
        utils::TupleElementType<1, utils::Tuple<int, double, char>>,
        double>);

    utils::Tuple const cvals2(3, 1, 'a');
    CHECK(utils::get<0>(cvals2) == 3);
    CHECK(utils::get<1>(cvals2) == 1);
    CHECK(utils::get<2>(cvals2) == 'a');

    struct Test{int a; int b; char c; int d;};
    CHECK(sizeof(utils::Tuple<int, int, char, int>) == sizeof(Test));

    CHECK(std::is_same_v<
        typename utils::TemplatedReverse<utils::Tuple<int, int, char>>::type,
        utils::Tuple<char, int, int>>);
    CHECK(std::is_same_v<
        typename utils::TemplatedReverse<std::pair<int, char>>::type,
        std::pair<char, int>>);

    utils::TupleUnion<Test, int, int, char, int> union0(1, 2, 'a', 3);
    std::vector<utils::TupleUnion<Test, int, int, char, int>> list;
    list.emplace_back();
    list.push_back(union0);
    CHECK(utils::get<0>(list[1]) == list[1]->a);
    CHECK(utils::get<1>(list[1]) == list[1]->b);
    CHECK(utils::get<2>(list[1]) == list[1]->c);
    CHECK(utils::get<3>(list[1]) == list[1]->d);

    typedef utils::Tuple<float, int> LeftTuple;
    typedef utils::Tuple<std::string, int> RightTuple;
    CHECK(std::is_same_v<
        utils::TupleCat<LeftTuple, RightTuple>::type,
        utils::Tuple<float, int, std::string, int>>);

TEST_FUNC_END


#include "utils/template_reverse.h"
TEST_FUNC_BEGIN("template reverse")

    constexpr utils::Tuple c1(1, 1, 'a');
    constexpr utils::Tuple c2(short(1));
    utils::Tuple<std::string> c3("hello");
    auto [a1, a2, a3, b1, c01] = utils::tuple_cat(c1, c2, c3).to_std();
    CHECK(a1 == 1);
    CHECK(a2 == 1);
    CHECK(c01 == "hello");

    constexpr utils::Tuple test_tuple = utils::reverse_parameter(1, 1.0f, 2.0);
    (void)test_tuple;

    auto [a, b, c] = utils::reverse_parameter(1, 1.0f, 2.0).to_std();
    CHECK(a == 2.0);
    CHECK(b == 1.0f);
    CHECK(c == 1);

TEST_FUNC_END


#include "utils/template_sort.h"
#ifdef max
#undef max
#endif
TEST_FUNC_BEGIN("template sort")

    utils::Max<int, 3, 6, 1, 10, 8> test_max1;
    CHECK(test_max1.index == 3);
    CHECK(test_max1.max == 10);


    using unsort_seq = utils::Sequence<int, 13, 6, 50, 10, 18>;
    using sorted_seq = utils::InsertionSort<unsort_seq>;
    constexpr auto sorted_tuple = utils::MakeTupleFromSequence<sorted_seq>;
    CHECK((sorted_tuple.to_std() == std::tuple(50, 18, 13, 10, 6)));

TEST_FUNC_END
