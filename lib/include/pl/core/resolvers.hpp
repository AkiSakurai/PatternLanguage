#pragma once

#include <pl/core/resolver.hpp>
#include <wolv/io/fs.hpp>

namespace pl::core::resolvers {

    using Result = hlp::Result<api::Source, std::string>;
    
    class FileResolver final {
    public:

        FileResolver() = default;
        explicit FileResolver(const std::vector<std::fs::path>& includePaths) : m_includePaths(includePaths) { }

        Result resolve(const std::string &path) const;

        [[nodiscard]] std::vector<std::fs::path> getIncludePaths() const {
            auto result = m_includePaths;
            if (!m_currentPatternPath.empty())
                result.push_back(m_currentPatternPath.parent_path());

            return result;
        }

        void setIncludePaths(const std::vector<std::fs::path> &includePaths) {
            this->m_includePaths = includePaths;
        }

        void setCurrentPatternPath(const std::fs::path &path) {
            m_currentPatternPath = path;
        }

        api::Source* addVirtualFile(const std::string &code, const std::string &path, bool mainSource = false) {
            this->m_virtualFiles[path] = api::Source(code, path, mainSource);
            return &this->m_virtualFiles[path];
        }

    private:
        std::fs::path m_currentPatternPath;
        std::vector<std::fs::path> m_includePaths;
        std::map<std::string, api::Source> m_virtualFiles;
    };
}