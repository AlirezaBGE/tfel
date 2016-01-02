/*!
 * \file   SchemeParserBase.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   21 déc. 2015
 */

#ifndef _LIB_MTEST_SCHEMEPARSERBASE_HXX_
#define _LIB_MTEST_SCHEMEPARSERBASE_HXX_

#include<memory>
#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MTest/Config.hxx"
#include"MTest/Types.hxx"

namespace mtest{

  // forward declaration
  struct SchemeBase;
  // forward declaration
  struct Evolution;
  
  struct MTEST_VISIBILITY_EXPORT SchemeParserBase
    : public tfel::utilities::CxxTokenizer
  {
    /*!
     * handle the @Author keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleAuthor(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @Date keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleDate(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @Description keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleDescription(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @PredictionPolicy keyword
     * \param[in,out] p : position in the input file
     */
    virtual void 
    handlePredictionPolicy(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @Times keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleTimes(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @Real keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleReal(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @StiffnessMatrixType keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleStiffnessMatrixType(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @StiffnessUpdatePolicy keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleStiffnessUpdatePolicy(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @UseCastemAccelerationAlgorithm keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleUseCastemAccelerationAlgorithm(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @CastemAccelerationTrigger keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleCastemAccelerationTrigger(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @CastemAccelerationAlgorithm keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleCastemAccelerationPeriod(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @AccelerationAlgorithm keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleAccelerationAlgorithm(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @AccelerationAlgorithmParameter keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleAccelerationAlgorithmParameter(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @MaximumNumberOfIterations keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleMaximumNumberOfIterations(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @MaximumNumberOfSubSteps keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleMaximumNumberOfSubSteps(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @Evolution keyword
     * \param[in,out] p : position in the input file
     */
    virtual void 
    handleEvolution(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @OutputFile keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleOutputFile(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @OutputFilePrecision keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleOutputFilePrecision(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @ResidualFile keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleResidualFile(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * handle the @ResidualFilePrecision keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleResidualFilePrecision(SchemeBase&,TokensContainer::const_iterator&);
    //! destructor
    ~SchemeParserBase();
  protected:
    /*!
     * \brief register call backs
     */
    virtual void registerCallBacks();
    /*!
     * \return everything from the given starting point up to the next
     * semi-colon.
     * \param[in,out] p : position in the input file
     */
    virtual std::string
    readUntilEndOfInstruction(TokensContainer::const_iterator&);
    /*!
     * \return a real value
     * \param[in] t : structure to be filled
     * \param[in,out] p : position in the input file
     */
    virtual real readDouble(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * read a time
     * \param[in,out] t : structure to be filled
     * \param[in,out] p : position in the input file
     */
    virtual real
    readTime(SchemeBase&,TokensContainer::const_iterator&);
    /*!
     * \brief parse an evolution
     * \param[in,out] t    : structure to be filled
     * \param[in]     type : evolution type
     * \param[in,out] p    : position in the input file
     */
    std::shared_ptr<Evolution>
    parseEvolution(SchemeBase&,
		   const std::string&,
		   TokensContainer::const_iterator&);
    /*!
     * \brief try to read the type of an evolution
     * \param[in,out] p : position in the input file
     */
    std::string
    readEvolutionType(TokensContainer::const_iterator&);
    /*!
     * read an array
     * The expected size of the array is given by the output vector
     * \param[out]    v : array to be read
     * \param[in,out] t : structure to be filled
     * \param[in,out] p : position in the input file
     */
    void
    readArrayOfSpecifiedSize(std::vector<real>&,
			     SchemeBase&,
			     TokensContainer::const_iterator&);
    /*!
     * \brief treat the current keyword
     * \param[in,out] p : position in the input file
     */
    bool treatKeyword(SchemeBase&,TokensContainer::const_iterator&);
    //! \return the list of keywords
    std::vector<std::string> getKeyWordsList(void) const;
    //! a simple alias
    using CallBack = void (SchemeParserBase::*)(SchemeBase&,
						TokensContainer::const_iterator&);
    //! callbacks
    std::map<std::string,CallBack> callbacks;
    //! input file
    std::string file;
  private:
    /*!
     * register a call back
     * \param[in] k : key word
     * \param[in] p : pointer to a member function
     */
    void registerCallBack(const std::string&,const CallBack&);
  }; // end of struct SchemeParserBase
  
} // end of namespace mtest

#endif /* _LIB_MTEST_SCHEMEPARSERBASE_HXX_ */
