#include "labellist.h"
#include <iostream>
using namespace std;

int main() {
    LabelList l1;

    LabelList::MsgToken newToken1 = l1.add("Hello");
    LabelList::MsgToken newToken2 = l1.add("Bye");
    l1.label(newToken1, "First");
    l1.label(newToken2, "First");

    LabelList::MsgToken newToken3 = l1.add("aaa");
    LabelList::MsgToken newToken4 = l1.add("bbb");
    LabelList::MsgToken newToken5 = l1.add("ccc");
    LabelList::MsgToken newToken6 = l1.add("ddd");
    l1.label(newToken3, "Second");
    l1.label(newToken4, "Second");
    l1.print(std::cout, "Second", ' ');
    l1.label(newToken5, "Second");
    l1.print(std::cout, "Second", ' ');
    l1.label(newToken6, "Second");
    l1.print(std::cout, "Second", ' ');
    newToken1.isLabelMember("First");
    l1.print(std::cout, "all", ' ');
    LabelList::MsgToken found = l1.find(0, "First");
    cout << found << endl;
    LabelList::MsgToken found1 = l1.find(3, "Second");
    cout << found1 << endl;
    l1.unlabel(newToken1, "First");
    l1.unlabel(newToken3, "Second");
    cout << l1.size() << endl;
    l1.remove(newToken1);
    cout << l1.size() << endl;
    l1.print(std::cout, "all", ' ');
    l1.print(std::cout, "First", ' ');
    l1.print(std::cout, "Second", ' ');
    // cout << l1.size() << endl;
    l1.clear();
    cout << l1.size() << endl;
    return 0;
}