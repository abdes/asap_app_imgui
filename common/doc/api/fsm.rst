.. Structure conventions
     # with overline, for parts
     * with overline, for chapters
     = for sections
     - for subsections
     ^ for sub-subsections
     " for paragraphs

*********************
Finite State Machines
*********************

The header <common/fsm.h> provides a template based framework to implement
finite state machines in C++.

.. doxygenclass:: asap::fsm::StateMachine

State Machine Actions
=====================

Actions are returned from state event handlers and are the normal way of
communicating back to the state machine what to do next. The only requirement on
an Action type is to have an Execute() method, that gets the state machine
instance, the current state and the event being processed.

Additional type management utility methods are added to action classes as needed
to simplify the unit testing of the states and actions. They are in no way used
in the basic functioning of a state machine which totally relies on static
compile-time types and rules.

.. doxygenstruct:: asap::fsm::TransitionTo
   :members:

.. doxygenstruct:: asap::fsm::DoNothing
   :members:

.. doxygenstruct:: asap::fsm::ReportError
   :members:

.. doxygenstruct:: asap::fsm::OneOf
   :members:

.. doxygenstruct:: asap::fsm::Maybe
   :members:

.. doxygenstruct:: asap::fsm::is_one_of

.. doxygenfunction:: asap::fsm::supports_alternative

State Machine Exceptions
========================

While state event handlers use primarily the asap::fsm::ReportError action to
transmit errors to the state machine, other places in the implementation such as
the OnEnter() or OnLeave() methods in states do not have access to actions, and
therefore report errors as exceptions.

.. doxygenclass:: asap::fsm::StateMachineError
   :members:

State Machine State Types
=========================

A number of helper base classes for implementing states are provided. These are
intended for simplifying typical obvious event processing cases and for allowing
to easily implement the unique interface of event handlers always returning an
Action.

.. doxygenstruct:: asap::fsm::On
   :members:

.. doxygenstruct:: asap::fsm::ByDefault
   :members:

.. doxygenstruct:: asap::fsm::Will
   :members:
