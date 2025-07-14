#ifndef ASHDESCRIPTORS_STATE_HPP
#define ASHDESCRIPTORS_STATE_HPP

#include "Ash/Buffer.hpp"
namespace ash::descriptors
{

    class State
    {
    public:
        //! @warning Please check if @param[in, out] Buffer is actually a non-nullptr.
        State(ash::Buffer* Buffer);

        bool HasBytesLeft(ash::Size Size);
        bool ConsumeBytes(ash::Size Size);

        //! @return Returns a nullptr if `HasBytesLeft` failed.
        void* ConsumeCheckAndReturnPreviousLocation(ash::Size Size);

        template <typename T>
        T* ConsumeCheckAndReturnPreviousLocationAs()
        {
            return static_cast<T*>(ConsumeCheckAndReturnPreviousLocation(sizeof(T)));
        }

        inline void* GetCurrentOffsetLocation()
        {
            if ((m_CurrentOffset + 1) > m_Buffer->GetSize())
            {
                return nullptr;
            }

            return m_Buffer->GetBufferAtOffset<void>(m_CurrentOffset);
        }

        inline ash::Buffer* GetBuffer()
        {
            return m_Buffer;
        }

        inline unsigned long long GetCurrentOffset()
        {
            return m_CurrentOffset;
        }

        inline unsigned long long GetBytesLeft()
        {
            return m_BytesLeft;
        }

    private:
        ash::Buffer* m_Buffer;
        unsigned long long m_CurrentOffset;
        unsigned long long m_BytesLeft;
    };

} // namespace ash::descriptors

#endif // !ASHDESCRIPTORS_STATE_HPP
