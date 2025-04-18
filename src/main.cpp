#include <QCoreApplication>
#include <QDebug>
#include <iostream>

namespace DoxygenTest {
/**
 * @class A
 * @brief A class 这是一个父类
 *
 */
class A {
public:
  /**
   * @brief 这是一个构造函数
   */
  A() {}
  /**
   * @brief 这是一个虚函数
   *
   * @param a 一个整数
   * @param b 另一个整数
   * @return a+b 返回两个整数的和
   */
  virtual int add(int a, int b) = 0;
};
/**
 * @class B
 * @brief B class 这是一个子类,继承自A
 *
 */
class B : A {
public:
  /**
   * @brief 这是一个构造函数
   * ## 这是一个markdown的标题
   *
   * \`\`\`cpp
   * // 这是一个代码块
   * int main() {
   * std::cout << "Hello World!";
   * }
   *
   * \`\`\`
   */
  B() {}
  /**
   * @brief 这是一个重写的函数
   *
   * @param a 一个整数
   * @param b 一个整数
   * @return a+b 返回两个整数的和
   */
  int add(int a, int b) override { return a + b; }
  /**
   * @brief  print Hi!!!
   */
  void print() { std::cout << "Hi!"; }
};

} // namespace DoxygenTest
/**
 * @brief 你好 这是main 函数
 *
 * @param argc  传递多少个参数
 * @param argv  传递的参数
 * @return int 返回值
 */
int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  qDebug() << "Hello, World!";
  return QCoreApplication::exec();
}
