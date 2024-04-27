/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow       
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------  
Licence:
  This file is part of phasicFlow code. It is a free software for simulating 
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions. 
 
  phasicFlow is distributed to help others in their research in the field of 
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/
#ifndef __simulationDomain_hpp__
#define __simulationDomain_hpp__

#include <array>

#include "domain.hpp"
#include "fileDictionary.hpp"
#include "span.hpp"
#include "streams.hpp"
#include "virtualConstructor.hpp"
#include "pointFlag.hpp"

namespace pFlow
{

class systemControl;


class simulationDomain
:
	public fileDictionary
{
private:	

	/// @brief acutal limits of the global box of  simulation
	box 			globalBox_;


	static constexpr uint32 sizeOfBoundaries_ = 6;

	static 
	inline const std::array<word,6>  boundaryNames_ = 
	{
		"left", "right", 
		"bottom", "top",
		"rear", "front"
	};
	
	virtual 
	bool createBoundaryDicts() = 0;

	virtual 
	bool setThisDomain() = 0;

public:

	// - type info
	TypeInfo("simulationDomain");

	/// Constrcut from components 
	explicit simulationDomain(systemControl& control);
	
	/// Destructor 
	~simulationDomain() override = default;
	

	create_vCtor
	(
		simulationDomain,
		systemControl,
		(systemControl& control),
		(control)	
	);

	const auto& globalBox()const
	{
		return globalBox_;
	}

	virtual 
	const dictionary& thisBoundaryDict()const = 0;
	
    virtual 
    bool initialUpdateDomains(span<realx3> pointPos) = 0;

    virtual
    uint32 initialNumberInThis()const = 0;

    virtual 
	bool initialTransferBlockData(
		span<char> src, 
		span<char> dst,
		size_t sizeOfElement) const = 0;
    
    virtual 
    bool initialTransferBlockData(
        span<realx3> src,
        span<realx3> dst) const = 0;
    
    virtual
    bool initialTransferBlockData(
        span<real> src,
        span<real> dst) const = 0;

    virtual
    bool initialTransferBlockData(
        span<uint32> src,
        span<uint32> dst) const = 0;
    
    virtual 
    bool initialTransferBlockData(
        span<int32> src,
        span<int32> dst) const = 0;
    
    
    template<typename T>
    bool initialTransferBlockData(
        span<T> src,
        span<T> dst )const
    {
        size_t el=sizeof(T);
        return initialTransferBlockData
        (
            charSpan(src), 
            charSpan(dst), 
            el
        );
    }

    /// @brief Number of points to be imported after updating domains 
	/// @return number of points
	virtual
	uint32 numberToBeImported()const = 0;

    virtual 
    uint32 numberToBeExported()const = 0;

	/// @brief Is this domain active?
	/// Active mean, there is particle in it and 
	/// boundaries and other entities of simulation domains are valid  
	virtual 
	bool domainActive()const = 0;

	
	/// @brief return the simulation domain of this processor 
	virtual 
	const domain& thisDomain()const = 0;
	
	
	domain extendThisDomain(
		const realx3& lowerPointExtension, 
		const realx3& upperPointExtension)const;

	/// @brief The original dictionary supplied by the user as input 
	inline 
	const auto& globalBoundaryDict()const
	{
		return static_cast<const fileDictionary&>(*this);
	}

	/// @brief return a const ref to dicrionary of boundary i of this processor 
	inline
	const dictionary& boundaryDict(uint32 i)const
	{
		return thisBoundaryDict().subDict(bundaryName(i));
	}
	
	/// @brief return a const ref to the plane of boundary i of this processor 
	const plane& boundaryPlane(uint32 i)const;
	

	static
	uniquePtr<simulationDomain> create(systemControl& control);

	/// @brief  Boundary name based on boundary index 
	/// @param i boundary index (range from 0 to 5) 
	/// @return const reference to name of the boundary 
	static inline
	const word& bundaryName(uint32 i)
	{
		return boundaryNames_[i];
	}

	static inline 
	uint32 sizeOfBoundaries() 
	{
		return boundaryNames_.size();
	}
	
}; // simulationDomain



}


#endif //__simulationDomain_hpp__
