#pragma once

#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>
#include <vector>

namespace udf {

  class tensor_node : public cdk::expression_node {
    std::shared_ptr<cdk::sequence_node> _elements; // Todas as expressões dos elementos (linearizados)

  public:
    tensor_node(int lineno, std::shared_ptr<cdk::sequence_node> elements)
        : cdk::expression_node(lineno), _elements(elements) {
    }

    std::shared_ptr<cdk::sequence_node> elements() const {
      return _elements;
    }

    void accept(basic_ast_visitor *sp, int lvl) override {
      sp->do_tensor_node(this, lvl);
    }
  };

} // namespace udf
