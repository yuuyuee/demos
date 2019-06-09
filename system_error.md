std::errc {};

class error_category; // virtual base class
    default_error_condition(errc) -> error_condition(errc, *this);
    name() = 0;
    message(errc) = 0;
    generic_category() -> class error_category: name = "generic";
    system_category() -> class error_category: name = "system";

class error_code(errc + error_category);
    error_code();
    error_code(errc, error_category);
    error_code(errc);
    assign(errc, error_category);
    value();
    category();
    message() -> error_category().message(ev);
    clear();
    default_error_condition() -> error_category().default_error_condition(errc);
    make_error_code(errc) -> error_code(errc, generic_error());

class error_condition(errc + error_category);
    error_condition( ev, error_category);
    error_condition(errc);
    assign(errc, error_category);
    value();
    category();
    message() -> error_category().message(errc);
    clear();
    make_error_condition(errc) ->error_condition(ev, generic_error());

class system_error: public runtime_error;
    system_error(error_code, what);
    system_error(error_code);
    system_error(errc, error_category, what);
    system_error(errc);
    code() -> error_code;
    what();

