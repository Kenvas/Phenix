#pragma once

#include "phenix/entry/EntryUtils.h"

namespace fnx
{
    namespace entry
    {
        class EntryManager final
        {
        private:
            SharedType Root_;
            SharedType Global_;
            StringType BootPath_;
            StringType Workspace_;
            SharedPtrs CurrentNodes_;
            size_t     ContextFlag_;

        public:
            EntryManager();

        public:
            SharedType GetEntry(StringType const & path);

            SharedType AddGlobalEntry(SharedType & entry);

            SharedType const & GetGlobal() const;

            StringType const & GetBootPath() const;

            void SetBootPath(StringType const & path);

            void SetWorkspace(StringType const & path);

            StringType const & GetWorkspace() const;

            SharedType const & GetRoot() const;

            SharedType const & GetCurrent() const;

            void SetCurrent(StringType const & path);

            void SetCurrent(SharedType const & current);

            void Run();

            void Exit();
        };

    }
}