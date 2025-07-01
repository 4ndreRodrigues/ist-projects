#pragma once

#include "targets/basic_ast_visitor.h"
#include <cdk/types/reference_type.h>


namespace udf {

  /**
   * Print nodes as XML elements to the output stream.
   */
  class type_checker: public basic_ast_visitor {
    cdk::symbol_table<udf::symbol> &_symtab;
    std::shared_ptr<udf::symbol> _function;

    basic_ast_visitor *_parent;

  public:
    type_checker(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<udf::symbol> &symtab, basic_ast_visitor *parent) :
        basic_ast_visitor(compiler), _symtab(symtab), _parent(parent) {
    }

  public:
    ~type_checker() {
      os().flush();
    }

  protected:
    void processUnaryExpression(cdk::unary_operation_node *const node, int lvl);
    void processBinaryExpression(cdk::binary_operation_node *const node, int lvl);
    template<typename T>
    void process_literal(cdk::literal_node<T> *const node, int lvl) {
    }

  protected:
    void do_IntOnlyExpression(cdk::binary_operation_node *const node, int lvl);
    void do_IDExpression(cdk::binary_operation_node *const node, int lvl);
    void do_PIDExpression(cdk::binary_operation_node *const node, int lvl);
    void do_ScalarLogicalExpression(cdk::binary_operation_node *const node, int lvl);
    void do_BooleanLogicalExpression(cdk::binary_operation_node *const node, int lvl);
    void do_GeneralLogicalExpression(cdk::binary_operation_node *const node, int lvl);

  public:
    // do not edit these lines
#define __IN_VISITOR_HEADER__
#include ".auto/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
    // do not edit these lines: end

  public:
    int checkPointersTypes(std::shared_ptr<cdk::basic_type> const left_type, std::shared_ptr<cdk::basic_type> const right_type) {
      int lt = 0, rt = 0;
      auto ltype = left_type;
      while (ltype->name() == cdk::TYPE_POINTER) {
        lt++;
        ltype = cdk::reference_type::cast(ltype)->referenced();
      }

      auto rtype = right_type;
      while (rtype->name() == cdk::TYPE_POINTER) {
        rt++;
        rtype = cdk::reference_type::cast(rtype)->referenced();
      }

      if((lt == rt) && (ltype->name() == rtype->name())) {
        //returns 1 if they are equal
        return 1;
      } else if((lt == rt) && (rtype->name() == cdk::TYPE_UNSPEC)) {
        //returns 2 if the right value is UNSPEC (probably due to being a stack alloc)
        return 2;
      } else {
        //returns 0 if types dont match
        return 0;
      }
    }
  };

} // udf

//---------------------------------------------------------------------------
//     HELPER MACRO FOR TYPE CHECKING
//---------------------------------------------------------------------------

#define CHECK_TYPES(compiler, symtab, node) { \
  try { \
    udf::type_checker checker(compiler, symtab, this); \
    (node)->accept(&checker, 0); \
  } \
  catch (const std::string &problem) { \
    std::cerr << (node)->lineno() << ": " << problem << std::endl; \
    return; \
  } \
}

#define ASSERT_SAFE_EXPRESSIONS CHECK_TYPES(_compiler, _symtab, node)
