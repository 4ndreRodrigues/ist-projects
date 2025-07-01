#pragma once

#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>

namespace udf {

  class tensor_reshape_node : public cdk::expression_node {
    cdk::expression_node *_tensor;
    cdk::sequence_node *_dimensions; // nova forma

  public:
    tensor_reshape_node(int lineno, cdk::expression_node *tensor, cdk::sequence_node *dims)
        : cdk::expression_node(lineno), _tensor(tensor), _dimensions(dims) {
    }

    cdk::expression_node *tensor() const {
      return _tensor;
    }

    cdk::sequence_node *dimensions() const {
      return _dimensions;
    }

    void accept(basic_ast_visitor *sp, int lvl) override {
      sp->do_tensor_reshape_node(this, lvl);
    }
  };

} // namespace udf
