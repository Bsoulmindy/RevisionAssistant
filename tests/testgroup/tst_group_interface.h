#ifndef TST_GROUP_INTERFACE_H
#define TST_GROUP_INTERFACE_H
#include <QTest>

class ITestGroup {
public:
    virtual void run_tests() = 0;
    virtual ~ITestGroup() {}
};

#endif // TST_GROUP_INTERFACE_H
