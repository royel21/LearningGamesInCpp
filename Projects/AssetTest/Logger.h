#ifndef _LOGGER_H
#define _LOGGER_H

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Plutus
{

	class Logger
	{
	public:

		template<typename... Args>
		inline static void info(Args &&...args) {
			if (!isinitialize) Init();
			s_CoreLogger->info(std::forward<Args>(args)...);
		}

		template<typename... Args>
		inline static void warn(Args &&...args) {
			if (!isinitialize) Init();
			s_CoreLogger->warn(std::forward<Args>(args)...);
		}

		template<typename... Args>
		inline static void error(Args &&...args) {
			if (!isinitialize) Init();
			s_CoreLogger->error(std::forward<Args>(args)...);
		}

	private:
		static bool isinitialize;
		static void Init();
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

} // namespace Plutus

#endif