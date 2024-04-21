Python 3.12.3 (tags/v3.12.3:f6650f9, Apr  9 2024, 14:05:25) [MSC v.1938 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license()" for more information.
>>> from animal_shelter import AnimalShelter
... animal_shelter = AnimalShelter()
... 
SyntaxError: multiple statements found while compiling a single statement
>>> from animal_shelter import AnimalShelter
Traceback (most recent call last):
  File "<pyshell#1>", line 1, in <module>
    from animal_shelter import AnimalShelter
ModuleNotFoundError: No module named 'animal_shelter'
