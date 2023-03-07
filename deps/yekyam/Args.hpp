#pragma once

#include <vector>
#include <string_view>
#include <optional>
#include <iostream>
#include <charconv>

namespace YAGP
{
	class Args
	{
		/*
		 * \fn	auto find_arg(const std::vector<std::string_view> &args, std::string_view option_name) const noexcept
		 * \brief Attempts to find the iterator to `option_name` in args
		 * \param std::string_view option_name The name of the option ('-name')
		 * \return An iterator to either the option_name in args or args.end()
		 */
		auto find_arg(std::string_view option_name) const noexcept
		{
			for (auto i = args.begin(); i != args.end(); i++)
			{
				if (*i == option_name)
					return i;
			}
			return args.end();
		}

		/*
		 * \fn std::optional<std::string_view> get_arg_value(std::string_view option_name) const noexcept
		 * \brief Attempts to get the `option_name`'s value in args ('-option_name value')
		 * \param std::string_view option_name The name of the option
		 * \return An std::optional containing the value or nullopt
		 */
		std::optional<std::string_view> get_arg_value(std::string_view option_name) const noexcept
		{
			auto it = find_arg(option_name);
			if (it + 1 != args.end())
				return *(it + 1);
			return std::nullopt;
		}

		std::vector<std::string_view> args;

	public:
		/*
		 * \fn Args(int argc, char **argv, size_t max = 10)
		 * \brief Constructor for Args
		 * \param int argc The number of arguments passed through the command line
		 * \param char **argv The actual arguments passed through the command line
		 * \param max The maximum allowed number of arguments, default is 10
		 * \note Assert is used here because argument parsing is the first thing in any program and defines program behavior.
		 * An error state here should in fact fail early.
		 */
		Args(int argc, char **argv, size_t max = 10)
		{
			assert(argc <= max && "Args error: argc greater than max (default is 10)");
			args = std::vector<std::string_view>(argv + 1, argv + argc);
		}

		/*
		 * \fn std::optional<T> get(std::string_view option_name, bool is_verbose = false) const noexcept;
		 * \brief Non specifed get template function. No implementation
		 * \param std::string_view option_name The name of the option
		 * \param bool is_verbose The verbosity of the function
		 * \return An std::optional containing the value or nullopt
		 */
		template <typename T>
		std::optional<T> get(std::string_view option_name, bool is_verbose = false) const noexcept;

		/*
		 * \fn std::optional<int> get(std::string_view option_name, bool is_verbose = false) const noexcept;
		 * \brief Attempts to get the int value specified by option_name
		 * \param std::string_view option_name The name of the option
		 * \param bool is_verbose The verbosity of the function
		 * \return An std::optional containing the int value or nullopt
		 */
		template <>
		std::optional<int> get<int>(std::string_view option_name, bool is_verbose) const noexcept
		{

			auto option_it = find_arg(option_name);

			if (option_it == args.end())
			{
				if (is_verbose)
					std::cout << "Couldn't get " << option_name << " option!\n";
				return std::nullopt;
			}

			auto str_view_option = get_arg_value(option_name);

			if (!str_view_option.has_value())
			{
				if (is_verbose)
					std::cout << "Couldn't get value of option " << option_name << "!\n";
				return std::nullopt;
			}

			auto str_view_value = str_view_option.value();
			std::cout << "value: " << str_view_value << '\n';

			int int_value{};
			auto [p, res] = std::from_chars(str_view_value.data(), str_view_value.data() + str_view_value.size(), int_value);
			if (res == std::errc{})
				return int_value;
			return std::nullopt;
		}

		/*
		 * \fn std::optional<float> get(std::string_view option_name, bool is_verbose = false) const noexcept;
		 * \brief Attempts to get the float value specified by option_name
		 * \param std::string_view option_name The name of the option
		 * \param bool is_verbose The verbosity of the function
		 * \return An std::optional containing the float value or nullopt
		 */
		template <>
		std::optional<float> get<float>(std::string_view option_name, bool is_verbose) const noexcept
		{

			auto option_it = find_arg(option_name);

			if (option_it == args.end())
			{
				if (is_verbose)
					std::cout << "Couldn't get " << option_name << " option!\n";
				return std::nullopt;
			}

			auto str_view_option = get_arg_value(option_name);

			if (!str_view_option.has_value())
			{
				if (is_verbose)
					std::cout << "Couldn't get value of option " << option_name << "!\n";
				return std::nullopt;
			}

			auto str_view_value = str_view_option.value();
			std::cout << "value: " << str_view_value << '\n';

			char *end;
			float f_value = std::strtof(str_view_value.data(), &end);
			if (str_view_value.data() == end)
			{
				if (is_verbose)
					std::cout << "Couldn't convert value " << str_view_value << " to float!\n";
				return std::nullopt;
			}
			return f_value;
		}

		/*
		 * \fn std::optional<bool> get(std::string_view option_name, bool is_verbose = false) const noexcept;
		 * \brief Attempts to get the bool value specified by option_name
		 * \param std::string_view option_name The name of the option
		 * \param bool is_verbose The verbosity of the function
		 * \return An std::optional containing the bool value or nullopt
		 */
		template <>
		std::optional<bool> get<bool>(std::string_view option_name, bool is_verbose) const noexcept
		{

			auto option_it = find_arg(option_name);

			if (option_it == args.end())
			{
				if (is_verbose)
					std::cout << "Couldn't get " << option_name << " option!\n";
				return std::nullopt;
			}

			return true;
		}

		/*
		 * \fn std::optional<std::string_view> get(std::string_view option_name, bool is_verbose = false) const noexcept;
		 * \brief Attempts to get the int value specified by option_name
		 * \param std::string_view option_name The name of the option
		 * \param bool is_verbose The verbosity of the function
		 * \return An std::optional containing the std::string_view value or nullopt
		 */
		template <>
		std::optional<std::string_view> get<std::string_view>(std::string_view option_name, bool is_verbose) const noexcept
		{

			auto option_it = find_arg(option_name);

			if (option_it == args.end())
			{
				if (is_verbose)
					std::cout << "Couldn't get " << option_name << " option!\n";
				return std::nullopt;
			}

			return get_arg_value(option_name);
		}
	};
};
