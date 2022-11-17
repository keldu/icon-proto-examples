program test_registry
  use mo_registry_interface
  use, intrinsic :: iso_c_binding
  implicit none

  type(descriptor_registry_ctx), pointer :: ctx

  print *, "Foo1"
  call create_context( ctx )

  print *, "Foo2"
  print *, "Foo3"

  call destroy_context( ctx )
  print *, "Foo4"
end program test_registry
