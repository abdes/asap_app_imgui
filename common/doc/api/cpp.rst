C++ language helpers
====================

Singleton class
------------------

The header <common/singleton.h> declares the class :class:`asap::Singleton`.

.. doxygenclass:: asap::Singleton
   :members:
   :protected-members:

Class :class:`asap::Singleton` provide a base class that can be used to implement concrete singleton
classes. A singleton is a class for which only one instance can exist in the process.

Logical traits
--------------

The header <common/traits/logical.h> defines equivalent of the C++ 17 logical
operations on type traits, reducing the requirement to just C++ 14.

.. doxygenstruct:: asap::conjunction

.. doxygenstruct:: asap::disjunction

.. doxygenstruct:: asap::negation

.. seealso::

   `std::conjunction <https://en.cppreference.com/w/cpp/types/conjunction>`_
      Documentation of the standard C++ conjunction operation.

   `std::disjunction <https://en.cppreference.com/w/cpp/types/disjunction>`_
      Documentation of the standard C++ disjunction operation.

   `std::negation <https://en.cppreference.com/w/cpp/types/negation>`_
      Documentation of the standard C++ negation operation.

Helper variables for C++ 14 and above
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If the C++ compiler provides support for C++ 14 or above, the following two
helper variables are also defined:

.. code-block:: c++

   template <class... Bi>
   constexpr bool conjunction_v = conjunction<Bi...>::value;

   template <class... Bi>
   constexpr bool disjunction_v = disjunction<Bi...>::value;

   template <class B>
   constexpr bool negation_v = negation<B>::value;

