.. Structure conventions
     # with overline, for parts
     * with overline, for chapters
     = for sections
     - for subsections
     ^ for sub-subsections
     " for paragraphs

**********************
Implementation Details
**********************

Behind the scene, the TextWrapper implementation uses a state machine based
tokenizer to transform the input text into a stream of tokens (word chunks,
white spaces and paragraph markers) to be wrapped by the text wrapper algorithm.

Tokenizer
=========

.. doxygenclass:: asap::wrap::detail::Tokenizer
   :members:
