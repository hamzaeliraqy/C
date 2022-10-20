using Lab4.Models;
using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Lab4.Data
{
    public class SchoolCommunityContext : DbContext
    {
        public SchoolCommunityContext(DbContextOptions<SchoolCommunityContext> options) : base(options)
        {
        }
        public DbSet<Student> Students { get; set; }
        public DbSet<Community> Communities { get; set; }
        public DbSet<CommunityMembership> CommunityMemberships { get;  set; }
        public DbSet<Lab4.Models.Ads> AdsImages { get; set; }
        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Lab4.Models.Ads>().ToTable("AdsImages");
            modelBuilder.Entity<Student>().ToTable("Students");
            modelBuilder.Entity<Community>().ToTable("Communities");
            modelBuilder.Entity<CommunityMembership>().HasKey(c => new { c.StudentID, c.CommunityID });
        }
       // ModelBuilder.Entity<CommunityMembership>().HasKey(c => new {c.StudentId, c.CommunityId});
      // modelBuilder.Entity<CommunityMembership>().HasKey(c => new { c.StudentId, c.CommunityId});

    }
}
