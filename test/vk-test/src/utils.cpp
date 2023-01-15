#include "test_header.h"
#include "utils/base_obj.h"
#include "utils/cloned_obj.h"
#include "utils/def_utils.h"
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

TEST_FUNC_BEGIN("utils")

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
