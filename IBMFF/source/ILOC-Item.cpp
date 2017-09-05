/*******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Jean-David Gadina - www.xs-labs.com / www.imazing.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

/*!
 * @file        ILOC-Item.hpp
 * @copyright   (c) 2017, Jean-David Gadina - www.xs-labs.com / www.imazing.com
 */

#include <IBMFF/ILOC.hpp>
#include <IBMFF/Parser.hpp>

template<>
class XS::PIMPL::Object< IBMFF::ILOC::Item >::IMPL
{
    public:
        
        IMPL( void );
        IMPL( const IMPL & o );
        ~IMPL( void );
        
        uint32_t                                 _itemID;
        uint8_t                                  _constructionMethod;
        uint16_t                                 _dataReferenceIndex;
        uint64_t                                 _baseOffset;
        std::vector< IBMFF::ILOC::Item::Extent > _extents;
};

#define XS_PIMPL_CLASS IBMFF::ILOC::Item
#include <XS/PIMPL/Object-IMPL.hpp>

namespace IBMFF
{
    ILOC::Item::Item( void ):
        XS::PIMPL::Object< Item >()
    {}
    
    ILOC::Item::Item( BinaryStream & stream, const ILOC & iloc ):
        XS::PIMPL::Object< Item >()
    {
        uint16_t count;
        uint16_t i;
        
        if( iloc.GetVersion() < 2 )
        {
            this->SetItemID( stream.ReadBigEndianUnsignedShort() );
        }
        else if( iloc.GetVersion() == 2 )
        {
            this->SetItemID( stream.ReadBigEndianUnsignedInteger() );
        }
        
        if( iloc.GetVersion() == 1 || iloc.GetVersion() == 2 )
        {
            this->SetConstructionMethod( static_cast< uint8_t >( stream.ReadBigEndianUnsignedShort() & 0xFF ) );
        }
        
        this->SetDataReferenceIndex( stream.ReadBigEndianUnsignedShort() );
        
        if( iloc.GetBaseOffsetSize() == 2 )
        {
            this->SetBaseOffset( stream.ReadBigEndianUnsignedShort() );
        }
        else if( iloc.GetBaseOffsetSize() == 4 )
        {
            this->SetBaseOffset( stream.ReadBigEndianUnsignedInteger() );
        }
        else if( iloc.GetBaseOffsetSize() == 8 )
        {
            this->SetBaseOffset( stream.ReadBigEndianUnsignedLong() );
        }
        
        count = stream.ReadBigEndianUnsignedShort();
        
        this->impl->_extents.clear();
        
        for( i = 0; i < count; i++ )
        {
            this->AddExtent( Extent( stream, iloc ) );
        }
    }
    
    uint32_t ILOC::Item::GetItemID( void ) const
    {
        return this->impl->_itemID;
    }
    
    uint8_t ILOC::Item::GetConstructionMethod( void ) const
    {
        return this->impl->_constructionMethod;
    }
    
    uint16_t ILOC::Item::GetDataReferenceIndex( void ) const
    {
        return this->impl->_dataReferenceIndex;
    }
    
    uint64_t ILOC::Item::GetBaseOffset( void ) const
    {
        return this->impl->_baseOffset;
    }
    
    void ILOC::Item::SetItemID( uint32_t value )
    {
        this->impl->_itemID = value;
    }
    
    void ILOC::Item::SetConstructionMethod( uint8_t value )
    {
        this->impl->_constructionMethod = value;
    }
    
    void ILOC::Item::SetDataReferenceIndex( uint16_t value )
    {
        this->impl->_dataReferenceIndex = value;
    }
    
    void ILOC::Item::SetBaseOffset( uint64_t value )
    {
        this->impl->_baseOffset = value;
    }
    
    std::vector< ILOC::Item::Extent > ILOC::Item::GetExtents( void ) const
    {
        return this->impl->_extents;
    }
    
    void ILOC::Item::AddExtent( const Extent & extent )
    {
        this->impl->_extents.push_back( extent );
    }
    
    void ILOC::Item::WriteDescription( std::ostream & os, std::size_t indentLevel ) const
    {
        std::string           i( ( indentLevel ) * 4, ' ' );
        std::vector< Extent > extents;
        
        extents = this->GetExtents();
        
        os << i << "{" << std::endl
           << i << "    Item ID:              " << this->GetItemID() << std::endl
           << i << "    Construction method:  " << static_cast< uint32_t >( this->GetConstructionMethod() ) << std::endl
           << i << "    Data reference index: " << this->GetDataReferenceIndex() << std::endl
           << i << "    Base offset:          " << this->GetBaseOffset() << std::endl
           << i << "    Extent count:         " << extents.size();
        
        if( extents.size() > 0 )
        {
            os << std::endl
               << i
               << "    {"
               << std::endl;
            
            for( const auto & extent: extents )
            {
                os << i << "        " << extent << std::endl;
            }
            
            os << i
               << "    }";
        }
        
        os << std::endl << i << "}";
    }
    
    std::ostream & operator << ( std::ostream & os, const ILOC::Item & item )
    {
        item.WriteDescription( os, 0 );
        
        return os;
    }
}

XS::PIMPL::Object< IBMFF::ILOC::Item >::IMPL::IMPL( void ):
    _itemID( 0 ),
    _constructionMethod( 0 ),
    _dataReferenceIndex( 0 ),
    _baseOffset( 0 )
{}

XS::PIMPL::Object< IBMFF::ILOC::Item >::IMPL::IMPL( const IMPL & o ):
    _itemID( o._itemID ),
    _constructionMethod( o._constructionMethod ),
    _dataReferenceIndex( o._dataReferenceIndex ),
    _baseOffset( o._baseOffset ),
    _extents( o._extents )
{}

XS::PIMPL::Object< IBMFF::ILOC::Item >::IMPL::~IMPL( void )
{}

