#pragma once

#include <string>
#include <memory>
#include <cdk/types/basic_type.h>

namespace udf {

  class symbol {
    bool _constant; // is it a constant? (not used but doesnt bother)
    int _qualifier; // qualifiers: public, external, "private" (i.e., none)
    std::shared_ptr<cdk::basic_type> _type;
    std::vector<std::shared_ptr<cdk::basic_type>> _argument_types;
    std::string _name;
    long _value; // hack!
    bool _initialized; // initialized?
    int _offset = 0; // 0 (zero) means global variable/function
    bool _function; // false for variables
    bool _declared = false;


  public:
    symbol(bool constant, int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &name, bool initialized,
           bool function, bool declared = false) :
        _constant(constant), _qualifier(qualifier), _type(type), _name(name), _value(0), _initialized(initialized), _function(
            function), _declared(declared) {
    }

    virtual ~symbol() {
      // EMPTY
    }

    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }
    void set_type(std::shared_ptr<cdk::basic_type> t) {
      _type = t;
    }
    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
    }
    const std::string &name() const {
      return _name;
    }
    const std::string& identifier() const {
      return name();
    }
    bool initialized() const {
      return _initialized;
    }
    long value() const {
      return _value;
    }
    long value(long v) {
      return _value = v;
    }
    bool constant() const {
      return _constant;
    }
    int qualifier() const {
      return _qualifier;
    }
    int offset() const {
      return _offset;
    }
    void set_offset(int offset) {
      _offset = offset;
    }
    bool isFunction() const {
      return _function;
    }
    bool global() const {
      return _offset == 0;
    }
    bool isVariable() const {
      return !_function;
    }
    bool declared() const {
      return _declared;
    }
    void set_argument_types(const std::vector<std::shared_ptr<cdk::basic_type>> &types) {
      _argument_types = types;
    }
    bool argument_is_typed(size_t ax, cdk::typename_type name) const {
      return _argument_types[ax]->name() == name;
    }
    std::shared_ptr<cdk::basic_type> argument_type(size_t ax) const {
      return _argument_types[ax];
    }
    size_t argument_size(size_t ax) const {
      return _argument_types[ax]->size();
    }
    size_t number_of_arguments() const {
      return _argument_types.size();
    }
  };

  inline auto make_symbol(bool constant, int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &name,
                          bool initialized, bool function, bool declared = false) {
    return std::make_shared<symbol>(constant, qualifier, type, name, initialized, function, declared);
  }

} // udf
