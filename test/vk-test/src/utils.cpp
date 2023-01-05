#include "test_header.h"
#include "utils/base_obj.h"
#include "utils/cloned_obj.h"

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
