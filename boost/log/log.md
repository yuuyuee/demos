## Logging sources

Base loggers
============
*#include <boost/log/sources/basic_logger.hpp>*
The simplest logging sources provided by the library are logger **logger**
and its thread-safe version **logger_mt**
These loggers only provide the ability to store **source-specific** attributes
within themselves and of course to from log records.

*Loggers with severity level support*
-----------------------------------
*#include <boost/log/sources/severity_feature.hpp>*
*#include <boost/log/sources/severity_logger.hpp>*
The ability to distinguish some log records from others based on some kind
of level of severity or importance is one of the most frequently requested
features. The class templates **severity_logger** and **severity_logger_mt**.
The loggers automatically register a special source-specific attribute
"Severity", which can be set for every record in a compact and efficient
manner, with a named argument severity that can be passed to the constructor
and/or the *open_record* method.

*Loggers with channel support*
----------------------------
*#include <boost/log/sources/channel_feature.hpp>*
*#include <boost/log/sources/channel_logger.hpp>*
Sometimes it important to associate log records with some application
component, such as the module or class name, the relation of the logged
information to some specific domain of application functionality or some
arbitrary tag that can be used later to route these records to a specific sink.
This feature is fulfilled with loggers **channel_logger**, **channel_logger_mt**.
TIP: For performance reasons it is advised to avoid dynamically setting the
channel name individually for every log record, when possible. Changing the
channel name involves dynamic memory allocation. Using distinct loggers for
different channels allows to avoid this overhead.

*Loggers with exception handling support*
---------------------------------------
*#include <boost/log/sources/exception_handler_feature.hpp>*
The library provides a logger feature that enables the user to handle and/or
suppress exceptions at the logger level. The *exception_handler* feature add
a *set_exception_handler* method to the logger that allows setting a function
object to be called if an exception is thrown from the logging core during
the filtering and processing of log records.
One can use the library-provided adapters to simplify implementing exception
handlers.
TIP: Logging core and sink frontends also support installing exception handlers.

*Loggers with mixed features*
---------------------------
*#include <boost/log/sources/severity_channel_logger.hpp>*
The library provides *severity_channel_logger* and *severity_channel_logger_mt*
 which combine features of the described loggers with *severity level* and
 *channels* support.
 The composite loggers are templates, too, which allows you to specify
 severity level and channel types. You can also design your own logger
 features and combine them with the ones provides by the library, as described
 in the *Extending the library* section.

 *Global storage for loggers*
 --------------------------
 *#include <boost/log/sources/global_logger_storage.hpp>*
The library provides a way to declare global loggers that cal be accessed pretty
much like *std::cout*. In fact. this feature can be used with any logger, including
user-defined ones. Having declared a global logger, one can be sure to have a
thread-safe access to this logger instance from any place of the application
code. The library also guarantees that a global logger instance will be unique
even across module boundaries. This allow employing logging even in header-only
components that may get compiled into different modules.
The easiest way to declare a global logger is to use the following macro:
*BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(mu_logger, src::severity_logger_mt<>)*
*BOOST_LOG_INLINE_GLOBAL_LOGGER_CTOR_ARGS(
    my_logger, src::severity_channel_logger<>,
    (keywords::severity = error)(keywords::channel = "my_channel"))*
*BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(
    // Do somthing that need to be done on logger initialization,
    // e.g. add a top wathc attribute.
    src::severity_logger_mt<> lg;
    lg.add_attribute("StopWatch", boost::make_shared<attrs::timer>());
    // The initializing routine must return the logger instance
    return lg;)*

## Core facilities
    1) Logging records
    2) Logging core

Logging records
===============
All the information that the logging library processes is packed into a
single object of type *record*.
All attached data, including the message text is represented as named
attribute values that can be fetched and processed by filters, formatters
and sinks.
Log records **cannot be copied, only moved**.
Log records MUST not be passed between different threads in multithreaded
environments.
*#include <boost/log/core/record.hpp>*

*Record views*
------------
While records are used for filling the information, the library use another
type to actually process it. Record views provide a similar interface to
records with a few notable distinctions:
    * Record views are **immutable**. This prevents formatters and sinks from
      modifying the record while it is being processed.
    * Record views are copyable. Since its contents are constant, the copy
      operation is shallow and therefore checp.
*#include <boost/log/core/record-view.hpp>*

Logging core
============
All methods of logging core are thread-safe in multithreaded environments.
    * Maintains global and thread-specific attribute sets.
    * Performs global filtering of log records.
    * Dispatches log records between sinks by applying sink-specific filters.
    * Provides a global hook for exception handlers.
    * Provides an entry point for log sources to put log records to.
    * Provides the flush method that can be used to enforce the
      synchronized state for all log sinks.
*#include <boost/log/core/core.hpp>*

*Attribute sets*
--------------
Add or remove global or thread-specific attributes to core:
    *add_global_attribute, remove_global_attribute*
    *add_thread_attribute, remove_thread_attribute*
Acquire a copy of the whole attribute set or install it into the core:
    *get_global_attributes, set_global_attributes*
    *get_thread_attributes, set_thread_attributes*

*Global filtering*
----------------
Handled by the filter function object to wipe out excessive log records
quickly:
    *set_filter, reset_filter*
    *set_logging_enabled*

*Sink management*
---------------
*add_sink, remove_sink*

*Exception handling*
------------------
*set_exception_handler*

*Feeding log records*
-------------------
One of the most important functions of the logging core is providing
an entry point for all logging sources to feed log records into.
This is done with the *open_record* and *push_record* methods.
    *open_record()*
    *push_record()*

## Sink frontends

Basic sink frontend services
=============================
    1) Filtering
    2) Formatting
    3) Exception handling

*Filtering*
---------
All sink frontend support filtering. The user can specify a custom filtering
function object or a filter constructed with the library provided tools.
The filter can be set with the *set_filter* method or cleared with the 
*reset_filter* method.
The filter is invoked during the call to the *will_consume* method that is
issued by the logging core.
If the filter is not set, it is assumed that the sink will accept any log record.
Note: Like the logging core, all sink frontends assume it is safe to call
filters from multiple threads concurrently. This is fine with the library
provided filters.

*Formatting*
----------
For text-based sink backends, frontends is implement record formatting.
Like with filters, lambda expressions can be used to construct formatters.
The formatter can be set for a text-based sink by calling the *set_formatter*
method or cleared by calling *reset_formatter*.

*Exception handling*
------------------

Unlocked sink frontend
=======================
*#include <boost/log/sinks/unlocked_frontend.hpp>*
The unlocked sink frontend implemented with *unlocked_sink* class template.
This is the only sink frontend available in a single threaded environment.

Synchronous sink frontend
==========================
*#include <boost/log/sinks/sync_frontend.hpp>*
The synchronous sink frontend is implemented with the *synchronous_sink*
class template.It is similar to the *unlocked_sink* but additionally provides
thread synchronization with a mutex before passing log records to the backend.

Asynchronous sink frontend
===========================
*#include <boost/log/sink/async_frontend.hpp>*
Related headers:
*#include <boost/log/sinks/unbounded_fifo_queue.hpp>*
*#include <boost/log/sinks/unbounded_ordering_queue.hpp>*
*#include <boost/log/sinks/bounded_fifo_queue.hpp>*
*#include <boost/log/sinks/bounded_ordering_queue.hpp>*
*#include <boost/log/sinks/drop_on_overflow.hpp>*
*#include <boost/log/sinks/block_on_overflow.hpp>*

The frontend is implemented in the *asynchronous_sink* class template.
Like the synchronous sink one, asynchronous sink frontend provides a way of
synchronizing access to the backend. All log records are passed to the
backend in a dedicated thread, which makes it suitable for backends that
may block for a considerable amount of time. The internal thread of the
frontend is spawned on the frontend constructor and joined on its destructor.
In order to stop the dedicated thread feeding log records to the backend one
can call the *stop* method of the frontend. This method will be called automatically
in the frontend destructor. The *stop* method, unlike thread interruption, will
only terminate the feeding loop when a log record that is being fed is processed
by the backend. However, it may happen that some records are still left in the
queue after returning from the *stop* method. In order to flush them to the backend
an additional call to the *feed_records* method is required. This is useful
in the application termination stage.
Spawning the dedicated thread for log record feeding can be suppressed with
the optional boolean *start_thread* named parameter of the frontend.
In this case the user can select either way of processing records:
    * Call the run method of the frontend. This call will block in the
      feeding loop. This loop can be interrupted with the call to *stop*.
    * Periodically call *feed_records*. This method will process all the log
      records that were in the fornt-end queue when the call was issued and
      then return.

*Customizing record queueing strategy*
------------------------------------
The *asynchronous_sink* class template can be customized with the record queueing
strategy. Several strategies are provided by the library:
    * *unbounded_fifo_queue*
    * *unbounded_ordering_queue*
    * *bounded_fifo_queue*
    * *bounded_ordering_queue*

Bounded queue support the following overflow strategies:
    * *drop_on_overflow*
    * *block_on_overflow*

## Sink backends

Text stream backend
====================
*#include <boost/log/sinks/text_ostream_backend.hpp>*
The text output stream sink backend is the most generic backend provided
by the library out of the box. The backend is implemented in the
*basic_text_ostream_backend* class template.
It supports formatting log records into strings and putting into one or
several streams. Each attached stream gets the same result of formatting,
so if you need to format log records differently for different streams, you
will need to create several sinks-each with its own formatter.
The backend also provides a feature that may come useful when debugging your
application. With the *auto_flush* method one can tell the sink to automatically
flush the buffers of all attached streams after each log record is written.
This will, of course, degrade logging performance, but in case of an application
crash there is a good chance that last log records will not be lost.

Text file backend
=================
*#include <boost/log/sinks/text_file_backend.hpp>*
Although it is possible to write logs into files with the *text stream backend*
the library also offers a special sink backend with an extended set of
features suitable for file-based logging. The features include:
    * Log file rotation based on file size and/or time
    * Flexible log file naming
    * Placing the rotated files into a special location in the file system
    * Deleting the oldest files in order to free more space on the file system
The backend is called *text_file_basckend*.

File rotation
-------------
1. Time point rotation: *rotation_at_time_point* class.
    * Every day rotation, at the specified time.
        *sinks::file::rotation_at_time_point(12, 0, 0)*
    * Rotation on the specified day of every week, at the specified time.
        *sinks::file::rotation_at_time_point(date_time::Tuesday, 0, 0, 0);*
        *sinks::file::rotation_at_time_point(date_time::Tuesday);*
    * Rotation on the specified day of each month, at the specified time.
        *sink::file::rotation_at_time_point(gregorian::greg_day(1), 0, 0, 0);*
        *sink::file:rotaion_at_time_point(gregorian::greg_day(1));*
2. Time interval rotations: *rotation_at_interval* class.
    *sinks::file::rotation_at_time_interval(posix_time::hours(1));*

Text multi-file backend
=======================
*#include <boost/log/sinks/text_multifile_backend.hpp>*
While the text stream and file backends are aimed to store all log records
into a single file/stream, this backend severs a different purpose. Assume
we have a banking request processing application and we want logs related to
every single request to be placed into a separate file. If we can associate
some attribute with the request identity then the *text_multifile_backend*
backend is the way to go.

Text IPC message queue backend
==============================
*#include <boost/log/sinks/text_ipc_message_queue_backend.hpp>*


Syslog backend
==============
*#include <boost/log/sinks/syslog_backend.hpp>*

## Lambda expressions

Generic attribute placeholder
=============================
*#include <boost/log/expressions/attr_fwd.hpp>*
*#include <boost/log/expressions/attr.hpp>*


Customizing fallback policy
===========================

Attribute tag and custom formatting operators
=============================================

Defining attribute keywords
===========================
*#include <boost/log/expressions/keyword_fwd.hpp>*
*#include <boost/log/expressions/keywords.hpp>*

Record placeholder
==================
*#include <boost/log/expressions/record.hpp>*

Message text placeholder
========================
*#include <boost/log/expression/message.hpp>*

## Predicate expressions

Attribute presence filter
=========================
*#include <boost/log/expressions/predicates/has_attr.hpp>*

Range checking filter
=====================
*#include <boost/log/expressions/predicates/is_in_range.hpp>*

Simple string matching filters
==============================
*#include <boost/log/expressions/predicates/begins_with.hpp>*
*#include <boost/log/expressions/predicates/ends_with.hpp>*
*#include <boost/log/expressions/predicates/contains.hpp>*

Advanced string matching filter
===============================
*#include <boost/log/expressions/predicates/matches.hpp>*
// Supporting headers
*#include <boost/log/support/regex.hpp>*
*#include <boost/log/support/std_regex.hpp>*
*#include <boost/log/support/xpressive.hpp>*
*#include <boost/log/support/spirit_qi.hpp>*
*#include <boost/log/support/spirit_classic.hpp>*

Severity threshold per channel filter
=====================================
*#include <boost/log/expressions/predicates/channel_severity_filter.hpp>*

Debugger presence filter
========================
*#include <boost/log/expressions/predicates/is_debugger_present.hpp>*

## Formatting expression
*#include <boost/log/expressions/formatters/format.hpp>*

Date and time formatter
=======================
*#include <boost/log/expressions/formatters/date_time.hpp>*
// Supporting headers
*#include <boost/log/support/date_time.hpp>*

Named scope formatter
=====================
*#include <boost/log/expressions/formatters/named_scope.hpp>*

Conditional formatters
======================
*#include <boost/log/expressions/formatters/if.hpp>*

## Character decorators

XML character decorator
=======================
*#include <boost/log/expressions/formatters/xml_decorator.hpp>*

CSV character decorator
=======================
*#include <boost/log/expressions/formatters/csv_decorator.hpp>*

C-style character decorators
============================
*#include <boost/log/expressions/formatters/c_decorator.hpp>*

General character decorator
===========================
*#include <boost/log/expressions/formatters/char_decorator.hpp>*

String length limiting decorator
================================
*#include <boost/log/expressions/formatters/max_size_decorator.hpp>*


## Attributes
*#include <boost/log/attributes/attribute.hpp>*
*#include <boost/log/attributes/attribute_cast.hpp>*
*#include <boost/log/attributes/attribute_value.hpp>*

Constants
=========
*#include <boost/log/attributes/constant.hpp>*

Mutable constants
=================
*#include <boost/log/attributes/mutable_constant.hpp>*

Counter
=======
*#include <boost/log/attributes/counter.hpp>*

Wall clock
==========
*#include <boost/log/attributes/clock.hpp>*

Stop watch(timer)
=================
*#include <boost/log/attributes/timer.hpp>*

Named scopes
============
*#include <boost/log/attributes/named_scope.hpp>*
// Supporting headers
*#include <boost/log/support/exception.hpp>*

Current process identifier
==========================
*#include <boost/log/attributes/current_process_id.hpp>*

Current process name
====================
*#include <boost/log/attributes/current_process_name.hpp>*

Current thread identifier
=========================
*#include <boost/log/attributes/current_thread_id.hpp>*

Function object as attributes
=============================
*#include <boost/log/attributes/function.hpp>*

Other attribute-related components
==================================

Attributes names
----------------
*#include <boost/log/attributes/attribute_name.hpp>*

Attributes value set
----------------
*#include <boost/log/attributes/attribute_value_set.hpp>*

Attribute value extraction and visitation
-----------------------------------------
Value visitation
*#include <boost/log/attributes/value_visitation_fwd.hpp>*
*#include <boost/log/attributes/value_visitation.hpp>*

Value extraction
*#include <boost/log/attributes/value_extraction_fwd.hpp>*
*#include <boost/log/attributes/value_extraction.hpp>*

Scoped attributes
*#include <boost/log/attributes/scoped_attribute.hpp>*

## Utilities

String literals
===============
*#include <boost/log/utility/string_literal.hpp>*

Type dispatchers
================
*#include <boost/log/utility/type_dispatch/type_dispatcher.hpp>*
Dynamic type dispatcher
*#include <boost/log/utility/type_dispatch/dynamic_type_dispatcher.hpp>*

Predefined type sequences
=========================
*#include <boost/log/utility/type_dispatch/standard_types.hpp>*
*#include <boost/utility/type_dispatch/date_time_types.hpp>*

Value reference wrapper
=======================
*#include <boost/log/utility/value_ref.hpp>*

Log record ordering
===================
*#include <boost/log/utility/record_ordering.hpp>*

Exception handlers
==================
*#include <boost/log/utility/exception_handler.hpp>*

output manipulators
===================

Customized logging manipulator
------------------------------
*#include <boost/log/utility/manipulators/to_log.hpp>*

Attribute value attaching manipulator
-------------------------------------
*#include <boost/log/utility/manipulators/add_value.hpp>*

Binary dump manipulator
-----------------------
*#include <boost/log/utility/manipulators/dump.hpp>*

Interprocess communication tools
================================

Resources name wrapper
----------------------
*#include <boost/log/utility/ipc/object_name.hpp>*

Reliable message queue
----------------------
*#include <boost/log/utility/ipc/reliable_message_queue.hpp>*


Simplified library initialization tools
=======================================

Convenience functions
---------------------
*#include <boost/log/utility/setup/console.hpp>*
*#include <boost/log/utility/setup/file.hpp>*
*#include <boost/log/utility/setup/common_attributes.hpp>*

Filter and formatter parsers
----------------------------
*#include <boost/log/utility/setup/filter_parser.hpp>*
*#include <boost/log/utility/set/formatter_parser.hpp>*

Library initialization from a settings container
------------------------------------------------
*#include <boost/log/utility/setup/settings.hpp>*
*#include <boost/log/utility/setup/from_settings.hpp>*

Library initialization from a settings file
-------------------------------------------
*#include <boost/log/utility/setup/from_stream.hpp>*

