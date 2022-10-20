using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Lab4.Models
{
    public class CommunityMembership
    {

        public Community Community { get; set; }
        public Student Student { get; set; }

        public int StudentID { get; set; }

        public string CommunityID { get; set; }

      
        //public IEnumerable<CommunityMembership> Memberships { get; set; }
        /*
        public Community Community { get; set; }
        public Student Student { get; set; }
        
        public string Title { get; set; }
        public bool IsMember { get; set; }*/

    }
}
