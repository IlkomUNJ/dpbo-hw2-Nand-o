# meson.build for HW2 Project

project('hw2_simulation', 'cpp',
  default_options : ['cpp_std=c++20'])

# Find all source files
sources = [
  'main.cpp',
  'library/Item/item.cpp',
  'library/Transaction/transaction.cpp',
  'library/Bank/bank.cpp',
  'library/Bank/bankreports.cpp',
  'library/Bank/bankTransaction.cpp',
  'library/User/user.cpp',
  'library/User/user_reports.cpp',
  'library/OnlineStore/onlinestore.cpp',
  'library/OnlineStore/serialization.cpp',
  'library/OnlineStore/reports.cpp',
  'Handlers/handlers.cpp',
  'Handlers/Auth/register.cpp',
  'Handlers/Auth/login.cpp',
  'Handlers/User/seller_menu.cpp',
  'Handlers/User/buyyer_menu.cpp',
  'Handlers/User/admin_menu.cpp',
]

# Define the include directories for headers
inc_dirs = [
  include_directories('library/Item'),
  include_directories('library/Transaction'),
  include_directories('library/Bank'),
  include_directories('library/User'),
  include_directories('library/OnlineStore'),
  include_directories('Handlers'),
  include_directories('Handlers/User'),
  include_directories('Handlers/Auth'),
]

# Define our executable
executable('simulasi',
  sources,
  include_directories : inc_dirs)
